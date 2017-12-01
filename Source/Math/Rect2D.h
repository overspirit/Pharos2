#pragma once

namespace Pharos
{
	namespace Math
	{
		// 2D矩形
		class Rect2D
		{
		public:
			//! 默认构造函数
			Rect2D()
			{
				left = 0;
				top = 0;
				right = 0;
				bottom = 0;
			}

			//! 使用left,top,right,bottom初始化值
			Rect2D(int32 left, int32 top, int32 right, int32 bottom)
			{
				this->left = left;
				this->top = top;
				this->right = right;
				this->bottom = bottom;
			}

			//! 使用两个点初始化值
			Rect2D(const Point2D& upperLeft, const Point2D& lowerRight)
			{
				left = upperLeft.x;
				top = upperLeft.y;
				right = lowerRight.x;
				bottom = lowerRight.y;
			}

			//! 使用点和尺寸赋值			
			Rect2D(const Point2D& pos, const Size2D& size)
			{
				left = pos.x;
				top = pos.y;
				right = pos.x + size.width;
				bottom = pos.y + size.height;
			}

			Rect2D(const Rect2D& other)
			{
				left = other.left;
				top = other.top;
				right = other.right;
				bottom = other.bottom;
			}

			//! 向右移动矩形
			Rect2D operator+(const Size2D& size) const
			{
				return Rect2D(left + size.width, top + size.height, right + size.width, bottom + size.height);
			}

			//! 向右移动矩形
			Rect2D& operator+=(const Size2D& size)
			{
				left += size.width;
				top += size.height;
				right += size.width;
				bottom += size.height;
				return *this;
			}

			//! 向左移动矩形
			Rect2D operator-(const Size2D& size) const
			{
				return Rect2D(left - size.width, top - size.height, right - size.width, bottom - size.height);
			}

			//! 向左移动矩形
			Rect2D& operator-=(const Size2D& size)
			{
				left -= size.width;
				top -= size.height;
				right -= size.width;
				bottom -= size.height;
				return *this;
			}

			//! 等于运算符重载
			bool operator==(const Rect2D& other) const
			{
				return ((left == other.left) && (top == other.top)
					&& (right == other.right) && (bottom == other.bottom));
			}

			//! 不等于运算符重载
			bool operator!=(const Rect2D& other) const
			{
				return ((left != other.left) || (top != other.top)
					|| (right != other.right) || (bottom != other.bottom));
			}

			//! 小于运算符重载
			bool operator<(const Rect2D& other) const
			{
				int32 width = right - left;
				int32 height = bottom - top;
				int32 otherWidth = other.right - other.left;
				int32 otherHeight = other.bottom - other.top;
				return (width * height) < (otherWidth * otherHeight);
			}

			//! 小于等于运算符重载
			bool operator<=(const Rect2D& other) const
			{
				int32 width = right - left;
				int32 height = bottom - top;
				int32 otherWidth = other.right - other.left;
				int32 otherHeight = other.bottom - other.top;
				return (width * height) <= (otherWidth * otherHeight);
			}

			//! 大于运算符重载
			bool operator>(const Rect2D& other) const
			{
				int32 width = right - left;
				int32 height = bottom - top;
				int32 otherWidth = other.right - other.left;
				int32 otherHeight = other.bottom - other.top;
				return (width * height) > (otherWidth * otherHeight);
			}

			//! 大于等于运算符重载
			bool operator>=(const Rect2D& other) const
			{
				int32 width = right - left;
				int32 height = bottom - top;
				int32 otherWidth = other.right - other.left;
				int32 otherHeight = other.bottom - other.top;
				return (width * height) >= (otherWidth * otherHeight);
			}

			//! 获得面积
			int32 GetArea() const
			{
				return (right - left) * (bottom - top);
			}

			//! 检测给定点是否在矩形范围内
			bool IsPointInside(const Point2D& pos) const
			{
				return (left <= pos.x && top <= pos.y && right >= pos.x && bottom >= pos.y);
			}

			//! 检测给定点是否在矩形范围内
			bool IsPointInside(int32 x, int32 y) const
			{
				return (left <= x && top <= y && right >= x && bottom >= y);
			}

			//! 检测两个矩形是否碰撞
			bool IsRectCollided(const Rect2D& other) const
			{
				return (bottom > other.top && top < other.bottom && right > other.left && left < other.right);
			}

			//! 使用另一个矩形剪裁本矩形
			void ClipAgainst(const Rect2D& other)
			{
				if (other.right < right) right = other.right;

				if (other.bottom < bottom) bottom = other.bottom;

				if (other.left > left) left = other.left;

				if (other.top > top) top = other.top;

				// correct possible invalid Rect2D resulting from clipping
				if (top > bottom) top = bottom;

				if (left > right) left = right;
			}

			//! 将本矩形移动到给定的矩形内，返回是否能包含本矩形
			bool ConstrainTo(const Rect2D& other)
			{
				int32 width = right - left;
				int32 height = bottom - top;
				int32 otherWidth = other.right - other.left;
				int32 otherHeight = other.bottom - other.top;
				if (otherWidth < width || otherHeight < height) return false;

				int32 diff = other.right - right;
				if (diff < 0)
				{
					right += diff;
					left += diff;
				}

				diff = other.bottom - bottom;
				if (diff < 0)
				{
					bottom += diff;
					top += diff;
				}

				diff = left - other.left;
				if (diff < 0)
				{
					left -= diff;
					right -= diff;
				}

				diff = top - other.top;
				if (diff < 0)
				{
					top -= diff;
					bottom -= diff;
				}

				return true;
			}

			//! 获得宽度
			int32 GetWidth() const
			{
				return right - left;
			}

			//! 获得高度
			int32 GetHeight() const
			{
				return bottom - top;
			}

			//! 修复，保证right大于left,bottom大于top
			void Repair()
			{
				if (right < left)
				{
					int32 t = right;
					right = left;
					left = t;
				}

				if (bottom < top)
				{
					int32 t = bottom;
					bottom = top;
					top = t;
				}
			}

			//! 是否有效，
			bool IsValid() const
			{
				return ((right >= left) && (bottom >= top));
			}

			//! 返回中心点
			Point2D GetCenter() const
			{
				return Point2D((left + right) / 2, (top + bottom) / 2);
			}

			//! 返回矩形大小
			Size2D GetSize() const
			{
				return Size2D(right - left, bottom - top);
			}

			//! 向矩形内添加一个点，如果点不在矩形内，矩形会增加尺寸适应该点
			void AddInternalPoint(const Point2D& p)
			{
				AddInternalPoint(p.x, p.y);
			}

			//! 向矩形内添加一个点，如果点不在矩形内，矩形会增加尺寸适应该点
			void AddInternalPoint(int32 x, int32 y)
			{
				if (x > right) right = x;

				if (y > bottom) bottom = y;

				if (x < left) left = x;

				if (y < top) top = y;
			}

			//检测是否为空
			bool IsEmpty() const
			{ 
				return ((left == 0) && (right == 0) && (top == 0) && (bottom == 0));
			}

			//清空矩形
			void Clear() { left = top = right = bottom = 0; }

		public:
			int32 left;
			int32 top;
			int32 right;
			int32 bottom;
		};
	}

	typedef Math::Rect2D Rect2Di;
}
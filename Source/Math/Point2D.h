#pragma once

namespace Pharos
{
	namespace Math
	{
		class Point2D
		{
		public:
			//! 默认构造函数
			Point2D()
			{
				x = 0;
				y = 0;
			}

			//! 用x和y初始化的构造函数
			Point2D(int32 nx, int32 ny)
			{
				x = nx;
				y = ny;
			}

			//! 使用相同的一个值初始化的构造函数
			explicit Point2D(int32 n)
			{
				x = n;
				y = n;
			}

			//! 拷贝构造函数
			Point2D(const Point2D& other)
			{
				x = other.x;
				y = other.y;
			}

			// 运算符重载
			Point2D operator-() const { return Point2D(-x, -y); }

			Point2D& operator=(const Point2D& other) { x = other.x; y = other.y; return *this; }

			Point2D operator+(const Point2D& other) const { return Point2D(x + other.x, y + other.y); }
			Point2D& operator+=(const Point2D& other) { x += other.x; y += other.y; return *this; }
			Point2D operator+(const int32 v) const { return Point2D(x + v, y + v); }
			Point2D& operator+=(const int32 v) { x += v; y += v; return *this; }

			Point2D operator-(const Point2D& other) const { return Point2D(x - other.x, y - other.y); }
			Point2D& operator-=(const Point2D& other) { x -= other.x; y -= other.y; return *this; }
			Point2D operator-(const int32 v) const { return Point2D(x - v, y - v); }
			Point2D& operator-=(const int32 v) { x -= v; y -= v; return *this; }

			Point2D operator*(const Point2D& other) const { return Point2D(x * other.x, y * other.y); }
			Point2D& operator*=(const Point2D& other) { x *= other.x; y *= other.y; return *this; }
			Point2D operator*(const int32 v) const { return Point2D(x * v, y * v); }
			Point2D& operator*=(const int32 v) { x *= v; y *= v; return *this; }

			Point2D operator/(const Point2D& other) const { return Point2D(x / other.x, y / other.y); }
			Point2D& operator/=(const Point2D& other) { x /= other.x; y /= other.y; return *this; }
			Point2D operator/(const int32 v) const { return Point2D(x / v, y / v); }
			Point2D& operator/=(const int32 v) { x /= v; y /= v; return *this; }

			bool operator==(const Point2D& other) const { return Equals(other); }
			bool operator!=(const Point2D& other) const { return !Equals(other); }

			//比较两个Point2D是否相等
			bool Equals(const Point2D& other) const
			{
				return (x == other.x) && (y == other.y);
			}

			//赋值
			Point2D& Set(int32 nx, int32 ny) { x = nx; y = ny; return *this; }
			Point2D& Set(const Point2D& p) { x = p.x; y = p.y; return *this; }
			
			//计算两个点之间的距离
			float32 GetDistanceFrom(const Point2D& other) const
			{
				float32 sx = (float32)(x - other.x);
				float32 sy = (float32)(y - other.y);
				return (float32)sqrt(sx*sx + sy*sy);
			}

			//计算两点之间距离的平方
			float32 GetDistanceFromSQ(const Point2D& other) const
			{
				float32 sx = (float32)(x - other.x);
				float32 sy = (float32)(y - other.y);
				return sx*sx + sy*sy;
			}

			//交换向量的两个值
			Point2D& Swap()
			{
				int32 temp = x;
				x = y;
				y = temp;
				return *this;
			}

			//求向量的绝对值
			Point2D& Abs()
			{
				x = abs(x);
				y = abs(y);
				return *this;
			}

			//清空值
			void Clear() { x = y = 0; }

		public:
			int32 x;
			int32 y;
		};
	}

	typedef Math::Point2D Point2Di;
}
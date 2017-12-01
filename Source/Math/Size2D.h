#pragma once

namespace Pharos
{
	namespace Math
	{
		// 2D空间内的距离
		class Size2D
		{
		public:
			Size2D()
			{
				this->width = 0;
				this->height = 0;
			}

			Size2D(int32 width, int32 height)
			{
				this->width = width;
				this->height = height;
			}

			//! 拷贝构造函数
			Size2D(const Size2D& other)
			{
				width = other.width;
				height = other.height;
			}

			//赋值运算符重载
			Size2D& operator=(const Size2D& other)
			{
				width = other.width;
				height = other.height;
				return *this;
			}

			//! 等于运算符重载
			bool operator==(const Size2D& other) const
			{
				return (width == other.width) && (height == other.height);
			}

			//! 不等于运算符
			bool operator!=(const Size2D& other) const
			{
				return (width != other.width) || (height != other.height);
			}

			//! 设置新值
			Size2D& Set(int32 width, int32 height)
			{
				this->width = width;
				this->height = height;
				return *this;
			}

			//! 除运算符重载
			Size2D& operator/=(int32 scale)
			{
				width /= scale;
				height /= scale;
				return *this;
			}

			//!  除运算符重载
			Size2D operator/(int32 scale) const
			{
				return Size2D(width / scale, height / scale);
			}

			//! 乘运算符重载
			Size2D& operator*=(int32 scale)
			{
				width *= scale;
				height *= scale;
				return *this;
			}

			//! 乘运算符重载
			Size2D operator*(int32 scale) const
			{
				return Size2D(width*scale, height*scale);
			}

			//! 加运算符重载
			Size2D& operator+=(const Size2D& other)
			{
				width += other.width;
				height += other.height;
				return *this;
			}

			//! 加运算符重载
			Size2D operator+(const Size2D& other) const
			{
				return Size2D(width + other.width, height + other.height);
			}

			//! 加运算符重载
			Size2D& operator+=(int32 value)
			{
				width += value;
				height += value;
				return *this;
			}

			//! 加运算符重载
			Size2D operator+(int32 value) const
			{
				return Size2D(width + value, height + value);
			}

			//! 减运算符重载
			Size2D& operator-=(const Size2D& other)
			{
				width -= other.width;
				height -= other.height;
				return *this;
			}

			//! 减运算符重载
			Size2D operator-(const Size2D& other) const
			{
				return Size2D(width - other.width, height - other.height);
			}

			//! 减运算符重载
			Size2D& operator-=(int32 value)
			{
				width -= value;
				height -= value;
				return *this;
			}

			//! 减运算符重载
			Size2D operator-(int32 value) const
			{
				return Size2D(width - value, height - value);
			}

			//! 小于等于运算符重载
			bool operator<=(const Size2D& other) const
			{
				return (width * height) <= (other.width * other.height);
			}

			//! 大于等于运算符重载
			bool operator>=(const Size2D& other) const
			{
				return (width * height) >= (other.width * other.height);
			}

			//! 小于运算符重载
			bool operator<(const Size2D& other) const
			{
				return (width * height) < (other.width * other.height);
			}

			//! 大于运算符重载
			bool operator>(const Size2D& other) const
			{
				return (width * height) > (other.width * other.height);
			}

			//! 获取面积
			int32 GetArea() const
			{
				return width*height;
			}

			//检测尺寸是否为空
			bool IsEmpty() const { return (width == 0) && (height == 0); }

			//强制赋值为0
			void Clear() { width = height = 0; }

		public:
			int32 width;
			int32 height;
		};
	}

	typedef Math::Size2D Size2Di;
}
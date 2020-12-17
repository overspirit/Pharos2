#pragma once

namespace Pharos
{
	namespace Math
	{
		// 2D向量		
		class Vector2D
		{
		public:
			//! 默认构造函数
			Vector2D()
			{
				x = 0;
				y = 0;
			}

			//! 用x和y初始化的构造函数
			Vector2D(float32 nx, float32 ny)
			{
				x = nx;
				y = ny;
			}

			//! 使用相同的一个值初始化的构造函数
			explicit Vector2D(float32 n)
			{
				x = n;
				y = n;
			}

			//! 拷贝构造函数
			Vector2D(const Vector2D& other)
			{
				x = (float32)other.x;
				y = (float32)other.y;
			}

			// 运算符重载
			Vector2D operator-() const { return Vector2D(-x, -y); }

			Vector2D& operator=(const Vector2D& other) { x = other.x; y = other.y; return *this; }

			Vector2D operator+(const Vector2D& other) const { return Vector2D(x + other.x, y + other.y); }
			Vector2D& operator+=(const Vector2D& other) { x += other.x; y += other.y; return *this; }
			Vector2D operator+(const float32 v) const { return Vector2D(x + v, y + v); }
			Vector2D& operator+=(const float32 v) { x += v; y += v; return *this; }

			Vector2D operator-(const Vector2D& other) const { return Vector2D(x - other.x, y - other.y); }
			Vector2D& operator-=(const Vector2D& other) { x -= other.x; y -= other.y; return *this; }
			Vector2D operator-(const float32 v) const { return Vector2D(x - v, y - v); }
			Vector2D& operator-=(const float32 v) { x -= v; y -= v; return *this; }

			Vector2D operator*(const Vector2D& other) const { return Vector2D(x * other.x, y * other.y); }
			Vector2D& operator*=(const Vector2D& other) { x *= other.x; y *= other.y; return *this; }
			Vector2D operator*(const float32 v) const { return Vector2D(x * v, y * v); }
			Vector2D& operator*=(const float32 v) { x *= v; y *= v; return *this; }

			Vector2D operator/(const Vector2D& other) const { return Vector2D(x / other.x, y / other.y); }
			Vector2D& operator/=(const Vector2D& other) { x /= other.x; y /= other.y; return *this; }
			Vector2D operator/(const float32 v) const { return Vector2D(x / v, y / v); }
			Vector2D& operator/=(const float32 v) { x /= v; y /= v; return *this; }

			bool operator==(const Vector2D& other) const { return Equals(other); }
			bool operator!=(const Vector2D& other) const { return !Equals(other); }

			operator float32* () { return &x; }
			operator const float32* () const { return &x; }

			//比较两个Vector2D是否相等
			bool Equals(const Vector2D& other) const
			{
				return equals(x, other.x) && equals(y, other.y);
			}

			//赋值
			Vector2D& Set(float32 nx, float32 ny) { x = nx; y = ny; return *this; }
			Vector2D& Set(const Vector2D& p) { x = p.x; y = p.y; return *this; }

			//获得向量长度
			float32 GetLength() const { return (float32)sqrt(x*x + y*y); }

			//获得向量长度的平方
			float32 GetLengthSQ() const { return x*x + y*y; }

			//计算两个向量的点积
			float32 DotProduct(const Vector2D& other) const
			{
				return x*other.x + y*other.y;
			}

			//! 计算和另一个向量的叉积,返回模长
			float32 CrossProduct(const Vector2D& other) const
			{
				return x*other.y - other.x*y;
			}

			//计算两个点之间的距离
			float32 GetDistanceFrom(const Vector2D& other) const
			{
				float32 sx = x - other.x;
				float32 sy = y - other.y;
				return (float32)sqrt(sx*sx + sy*sy);
			}

			//计算两点之间距离的平方
			float32 GetDistanceFromSQ(const Vector2D& other) const
			{
				float32 sx = x - other.x;
				float32 sy = y - other.y;
				return sx*sx + sy*sy;
			}

			//交换向量的两个值
			Vector2D& Swap()
			{
				float32 temp = x;
				x = y;
				y = temp;
				return *this;
			}

			//求向量的绝对值
			Vector2D& Abs()
			{
				x = abs(x);
				y = abs(y);
				return *this;
			}

			//向量归一化,浮点数有效
			Vector2D& Normalize()
			{
				float32 length = (float32)(x*x + y*y);
				if (length == 0) return *this;

				length = (float32)(1.0 / sqrt(length));
				x = (float32)(x * length);
				y = (float32)(y * length);
				return *this;
			}

			//以指定点为中心点旋转指定角度，radian为弧度制，返回本向量
			Vector2D& RotateBy(float64 radian, const Vector2D& center = Vector2D())
			{
				const float64 cs = cos(radian);
				const float64 sn = sin(radian);

				x -= center.x;
				y -= center.y;

				Set((float32)(x*cs - y*sn), (float32)(x*sn + y*cs));

				x += center.x;
				y += center.y;

				return *this;
			}

			//以x轴向右为0度，以y轴向上为正角度，返回向量指向的角度，返回单位为弧度制
			float64 GetAngleTrig() const
			{
				if (y == 0) return x < 0 ? PI : 0;
				else if (x == 0) return y < 0 ? (PI * 1.5) : (PI * 0.5);

				if (y > 0)
				{
					if (x > 0) return atan((float64)y / (float64)x);
					else return PI - atan((float64)y / -(float64)x);
				}
				else
				{
					if (x > 0) return (PI * 2.0) - atan(-(float64)y / (float64)x);
					else return PI + atan(-(float64)y / -(float64)x);
				}
			}

			//以x轴向右为0度，以y轴向下为正角度，返回向量指向的角度，返回单位为弧度制
			inline float64 GetAngle() const
			{
				if (y == 0) return x < 0 ? PI : 0;
				else if (x == 0) return y < 0 ? (PI * 0.5) : (PI * 1.5);

				// don't use getLength here to avoid precision loss with int32 vectors
				// avoid floating-point trouble as sqrt(y*y) is occasionally larger than y, so clamp
				const float64 tmp = clamp(y / sqrt((float64)(x*x + y*y)), -1.0, 1.0);
				const float64 angle = atan(sqrt(1 - tmp*tmp) / tmp);

				if (x > 0 && y > 0) return angle + (PI * 1.5);
				else if (x > 0 && y < 0) return angle + (PI * 0.5);
				else if (x < 0 && y < 0) return (PI * 0.5) - angle;
				else if (x < 0 && y>0) return (PI * 1.5) - angle;

				return angle;
			}

			//返回两个向量之间的夹角，返回单位为弧度制,范围为0,180
			inline float64 GetAngleWith(const Vector2D& b) const
			{
				float64 tmp = (float64)(x*b.x + y*b.y);

				if (tmp == 0.0) return PI * 0.5;

				tmp = tmp / sqrt((float64)((x*x + y*y) * (b.x*b.x + b.y*b.y)));

				//   avoid floating-point trouble
				if (tmp > 1.0) tmp = 1.0;

				float64 a = atan(sqrt(1 - tmp*tmp) / tmp);
				return a > 0 ? a : (PI + a);
			}

			//判断这个点是否在给定的两点之间
			bool IsBetweenPoints(const Vector2D& begin, const Vector2D& end) const
			{
				if (begin.x != end.x)
				{
					return ((begin.x <= x && x <= end.x) ||
						(begin.x >= x && x >= end.x));
				}
				else
				{
					return ((begin.y <= y && y <= end.y) ||
						(begin.y >= y && y >= end.y));
				}
			}

			//本向量和给定向量线性插值，返回一个新的向量
			Vector2D GetInterpolated(const Vector2D& other, float32 d) const
			{
				float32 inv = 1.0f - d;
				return Vector2D(other.x*inv + x*d, other.y*inv + y*d);
			}

			//插值，使用给定的两个向量和本向量进行二次方插值，返回一个新的向量
			Vector2D GetInterpolatedQuadratic(const Vector2D& v2, const Vector2D& v3, float32 d) const
			{
				// this*(1-d)*(1-d) + 2 * v2 * (1-d) + v3 * d * d;
				const float32 inv = 1.0f - d;
				const float32 mul0 = inv * inv;
				const float32 mul1 = 2.0f * d * inv;
				const float32 mul2 = d * d;

				return Vector2D(x * mul0 + v2.x * mul1 + v3.x * mul2,	y * mul0 + v2.y * mul1 + v3.y * mul2);
			}

			//插值，利用给定的两个向量线性插值，会修改本向量
			Vector2D& Interpolate(const Vector2D& a, const Vector2D& b, float32 d)
			{
				x = a.x + (b.x - a.x) * d;
				y = a.y + (b.y - a.y) * d;
				return *this;
			}

			//检测是否为空
			bool IsEmpty() const { return (iszero(x) && iszero(y)); }

			//清空值
			void Clear() { x = y = 0; }

		public:
			float32 x;
			float32 y;
		};
	}

	typedef Math::Vector2D Vector2Df;
}
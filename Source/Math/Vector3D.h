#pragma once

namespace Pharos
{
	namespace Math
	{
		// 3D向量
		class Vector3D
		{
		public:
			//! 默认构造函数
			Vector3D()
			{
				x = 0;
				y = 0;
				z = 0;
			}

			//! 使用3个不同的值赋值
			Vector3D(float32 nx, float32 ny, float32 nz)
			{
				x = nx;
				y = ny;
				z = nz;
			}

			//! 使用一个相同的值赋值
			explicit Vector3D(float32 n)
			{
				x = n;
				y = n;
				z = n;
			}

			//! 拷贝构造函数
			Vector3D(const Vector3D& other)
			{
				x = (float32)other.x;
				y = (float32)other.y;
				z = (float32)other.z;
			}

			// 运算符重载

			Vector3D operator-() const { return Vector3D(-x, -y, -z); }

			Vector3D& operator=(const Vector3D& other) { x = other.x; y = other.y; z = other.z; return *this; }

			Vector3D operator+(const Vector3D& other) const { return Vector3D(x + other.x, y + other.y, z + other.z); }
			Vector3D& operator+=(const Vector3D& other) { x += other.x; y += other.y; z += other.z; return *this; }
			Vector3D operator+(const float32 val) const { return Vector3D(x + val, y + val, z + val); }
			Vector3D& operator+=(const float32 val) { x += val; y += val; z += val; return *this; }

			Vector3D operator-(const Vector3D& other) const { return Vector3D(x - other.x, y - other.y, z - other.z); }
			Vector3D& operator-=(const Vector3D& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
			Vector3D operator-(const float32 val) const { return Vector3D(x - val, y - val, z - val); }
			Vector3D& operator-=(const float32 val) { x -= val; y -= val; z -= val; return *this; }

			Vector3D operator*(const Vector3D& other) const { return Vector3D(x * other.x, y * other.y, z * other.z); }
			Vector3D& operator*=(const Vector3D& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
			Vector3D operator*(const float32 v) const { return Vector3D(x * v, y * v, z * v); }
			Vector3D& operator*=(const float32 v) { x *= v; y *= v; z *= v; return *this; }

			Vector3D operator/(const Vector3D& other) const { return Vector3D(x / other.x, y / other.y, z / other.z); }
			Vector3D& operator/=(const Vector3D& other) { x /= other.x; y /= other.y; z /= other.z; return *this; }
			Vector3D operator/(const float32 v) const { float32 i = (float32)1.0 / v; return Vector3D(x * i, y * i, z * i); }
			Vector3D& operator/=(const float32 v) { float32 i = (float32)1.0 / v; x *= i; y *= i; z *= i; return *this; }

			bool operator==(const Vector3D& other) const { return Equals(other); }
			bool operator!=(const Vector3D& other) const { return !Equals(other); }

			operator float32* () { return &x; }
			operator const float32* () const { return &x; }

			//! 比较
			bool Equals(const Vector3D& other) const
			{
				return (equals(x, other.x) && equals(y, other.y) && equals(z, other.z));
			}

			//赋值
			Vector3D& Set(float32 nx, float32 ny, float32 nz) { x = nx; y = ny; z = nz; return *this; }
			Vector3D& Set(const Vector3D& p) { x = p.x; y = p.y; z = p.z; return *this; }

			//! 获得向量长度
			float32 GetLength() const
			{
				return (float32)sqrt(x*x + y*y + z*z);
			}

			//! 获得向量长度的平方
			float32 GetLengthSQ() const
			{
				return x*x + y*y + z*z;
			}

			//! 和另一个向量计算点积
			float32 DotProduct(const Vector3D& other) const
			{
				return x*other.x + y*other.y + z*other.z;
			}

			//! 获得和另一个点的距离
			float32 GetDistanceFrom(const Vector3D& other) const
			{
				float32 sx = x - other.x;
				float32 sy = y - other.y;
				float32 sz = z - other.z;
				return (float32)sqrt(sx*sx + sy*sy + sz*sz);
			}

			//! 获得和另一个点距离的平方
			float32 GetDistanceFromSQ(const Vector3D& other) const
			{
				float32 sx = x - other.x;
				float32 sy = y - other.y;
				float32 sz = z - other.z;
				return sx*sx + sy*sy + sz*sz;
			}

			//求向量的绝对值
			Vector3D& Abs()
			{
				x = abs(x);
				y = abs(y);
				z = abs(z);
				return *this;
			}

			//! 计算和另一个向量的叉积
			Vector3D CrossProduct(const Vector3D& p) const
			{
				return Vector3D(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
			}

			//! 返回本向量的一个正交向量
			Vector3D GetOrthoVector() const
			{
				float64 xAbs = fabs(x);
				float64 yAbs = fabs(y);
				float64 zAbs = fabs(z);

				Vector3D vect;
				if (xAbs > yAbs)
				{
					if (xAbs > zAbs) vect.z = 1.0f;
					else vect.y = 1.0f;
				}
				else
				{
					if (yAbs > zAbs) vect.x = 1.0f;
					else vect.y = 1.0f;
				}

				Vector3D orthoVect = CrossProduct(vect);
				orthoVect.Normalize();
				return orthoVect;
			}

			//! 本向量是否在给定的两个向量中间
			bool IsBetweenPoints(const Vector3D& begin, const Vector3D& end) const
			{
				float32 sx = end.x - begin.x;
				float32 sy = end.y - begin.y;
				float32 sz = end.z - begin.z;
				const float32 f = sx*sx + sy*sy + sz*sz;
				return (GetDistanceFromSQ(begin) <= f) && (GetDistanceFromSQ(end) <= f);
			}

			//! 向量归一化,浮点数有效
			Vector3D& Normalize()
			{
				float64 length = x*x + y*y + z*z;

				// this check isn't an optimization but prevents getting NAN in the sqrt.
				if (length == 0) return *this;

				length = 1.0 / sqrt(length);

				x = (float32)(x * length);
				y = (float32)(y * length);
				z = (float32)(z * length);

				return *this;
			}

			//!	产生一个随机的单位向量,float32为浮点数时才有效
			Vector3D& RandUnit()
			{
				x = (float32)(random((float32)-1, (float32)1));
				y = (float32)(random((float32)-1, (float32)1));
				z = (float32)(random((float32)-1, (float32)1));
				return Normalize();
			}

			//! 设置该向量的长度
			Vector3D& SetLength(float32 newlength)
			{
				Normalize();
				return (*this *= newlength);
			}

			//! 取该向量的相反向量
			Vector3D& Invert()
			{
				x *= -1;
				y *= -1;
				z *= -1;
				return *this;
			}

			//! 以给定点为旋转点，旋转该向量的x和z值到指定的角度，degrees为弧度制
			void RotateXZBy(float32 degrees, const Vector3D& center = Vector3D())
			{
				float64 cs = cos(degrees);
				float64 sn = sin(degrees);

				x -= center.x;
				z -= center.z;

				Set((float32)(x*cs - z*sn), y, (float32)(x*sn + z*cs));

				x += center.x;
				z += center.z;
			}

			//! 以给定点为旋转点，旋转该向量的x和y值到指定的角度，degrees为弧度制
			void RotateXYBy(float32 degrees, const Vector3D& center = Vector3D())
			{
				float64 cs = cos(degrees);
				float64 sn = sin(degrees);

				x -= center.x;
				y -= center.y;

				Set((float32)(x*cs - y*sn), (float32)(x*sn + y*cs), z);

				x += center.x;
				y += center.y;
			}

			//! 以给定点为旋转点，旋转该向量的y和z值到指定的角度，degrees为弧度制
			void RotateYZBy(float32 degrees, const Vector3D& center = Vector3D())
			{
				float64 cs = cos(degrees);
				float64 sn = sin(degrees);

				z -= center.z;
				y -= center.y;

				Set(x, (float32)(y*cs - z*sn), (float32)(y*sn + z*cs));

				z += center.z;
				y += center.y;
			}

			//!本向量和给定的向量线性插值，返回一个新的向量
			Vector3D GetInterpolated(const Vector3D& other, float32 d) const
			{
				const float32 inv = 1.0f - d;
				return Vector3D((other.x*inv + x*d), (other.y*inv + y*d), (other.z*inv + z*d));
			}

			//! 本向量和给定的两个向量二次插值，返回一个新的向量
			Vector3D GetInterpolatedQuadratic(const Vector3D& v2, const Vector3D& v3, float32 d) const
			{
				// this*(1-d)*(1-d) + 2 * v2 * (1-d) + v3 * d * d;
				const float32 inv = 1.0f - d;
				const float32 mul0 = inv * inv;
				const float32 mul1 = 2.0f * d * inv;
				const float32 mul2 = d * d;

				return Vector3D
				(
					(x * mul0 + v2.x * mul1 + v3.x * mul2),
					(y * mul0 + v2.y * mul1 + v3.y * mul2),
					(z * mul0 + v2.z * mul1 + v3.z * mul2)
				);
			}

			//! 插值，使用给定的两个向量线性插值，返回修改过的本向量
			Vector3D& Interpolate(const Vector3D& a, const Vector3D& b, float32 d)
			{
				x = a.x + (b.x - a.x) * d;
				y = a.y + (b.y - a.y) * d;
				z = a.z + (b.z - a.z) * d;
				return *this;
			}

			//检测是否为空
			bool IsEmpty() const { return (iszero(x) && iszero(y) && iszero(z)); }

			//清空
			void Clear() { x = y = z = 0; }

		public:
			float32 x;
			float32 y;
			float32 z;
		};
	}

	typedef Math::Vector3D Vector3Df;
}
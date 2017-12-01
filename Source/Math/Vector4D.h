#pragma once

namespace Pharos
{
	namespace Math
	{
		// 4D向量
		class Vector4D
		{
		public:
			//! 默认构造函数
			Vector4D()
			{
				x = 0;
				y = 0;
				z = 0;
				w = 0;
			}

			//! 使用3个不同的值赋值
			Vector4D(float32 nx, float32 ny, float32 nz, float32 nw)
			{
				x = nx;
				y = ny;
				z = nz;
				w = nw;
			}

			//! 使用一个相同的值赋值
			explicit Vector4D(float32 n)
			{
				x = n;
				y = n;
				z = n;
				w = n;
			}

			//! 拷贝构造函数
			Vector4D(const Vector4D& other)
			{
				x = (float32)other.x;
				y = (float32)other.y;
				z = (float32)other.z;
				w = (float32)other.w;
			}

			// 运算符重载

			Vector4D operator-() const { return Vector4D(-x, -y, -z, -w); }

			Vector4D& operator=(const Vector4D& other) { x = other.x; y = other.y; z = other.z; w = other.w; return *this; }

			Vector4D operator+(const Vector4D& other) const { return Vector4D(x + other.x, y + other.y, z + other.z, w + other.w); }
			Vector4D& operator+=(const Vector4D& other) { x += other.x; y += other.y; z += other.z; w += other.w; return *this; }
			Vector4D operator+(const float32 val) const { return Vector4D(x + val, y + val, z + val, w + val); }
			Vector4D& operator+=(const float32 val) { x += val; y += val; z += val; w += val; return *this; }

			Vector4D operator-(const Vector4D& other) const { return Vector4D(x - other.x, y - other.y, z - other.z, w - other.w); }
			Vector4D& operator-=(const Vector4D& other) { x -= other.x; y -= other.y; z -= other.z; w -= other.w; return *this; }
			Vector4D operator-(const float32 val) const { return Vector4D(x - val, y - val, z - val, w - val); }
			Vector4D& operator-=(const float32 val) { x -= val; y -= val; z -= val; w -= val; return *this; }

			Vector4D operator*(const Vector4D& other) const { return Vector4D(x * other.x, y * other.y, z * other.z, w * other.w); }
			Vector4D& operator*=(const Vector4D& other) { x *= other.x; y *= other.y; z *= other.z; w *= other.w; return *this; }
			Vector4D operator*(const float32 v) const { return Vector4D(x * v, y * v, z * v, w * v); }
			Vector4D& operator*=(const float32 v) { x *= v; y *= v; z *= v; w *= v; return *this; }

			Vector4D operator/(const Vector4D& other) const { return Vector4D(x / other.x, y / other.y, z / other.z, w / other.w); }
			Vector4D& operator/=(const Vector4D& other) { x /= other.x; y /= other.y; z /= other.z; w /= other.w; return *this; }
			Vector4D operator/(const float32 v) const { float32 i = (float32)1.0 / v; return Vector4D(x * i, y * i, z * i, w * i); }
			Vector4D& operator/=(const float32 v) { float32 i = (float32)1.0 / v; x *= i; y *= i; z *= i; w *= i; return *this; }

			//! use weak float32 compare
			bool operator==(const Vector4D& other) const { return Equals(other); }
			bool operator!=(const Vector4D& other) const { return !Equals(other); }

			operator float32* () { return &x; }
			operator const float32* () const { return &x; }

			//! 比较
			bool Equals(const Vector4D& other) const
			{
				return (equals(x, other.x) && equals(y, other.y) && equals(z, other.z) && equals(w, other.w));
			}

			//赋值
			Vector4D& Set(float32 nx, float32 ny, float32 nz, float32 nw) { x = nx; y = ny; z = nz; w = nw; return *this; }
			Vector4D& Set(const Vector4D& p) { x = p.x; y = p.y; z = p.z; w = p.w; return *this; }

			//! 获得向量长度
			float32 GetLength() const { return (float32)sqrt(x*x + y*y + z*z + w*w); }

			//! 获得向量长度的平方
			float32 GetLengthSQ() const { return x*x + y*y + z*z + w*w; }

			//! 和另一个向量计算点积
			float32 DotProduct(const Vector4D& other) const
			{
				return x*other.x + y*other.y + z*other.z + w*other.w;
			}

			//! 获得和另一个点的距离
			float32 GetDistanceFrom(const Vector4D& other) const
			{
				float32 sx = x - other.x;
				float32 sy = y - other.y;
				float32 sz = z - other.z;
				float32 sw = w - other.w;
				return (float32)sqrt(sx*sx + sy*sy + sz*sz + sw*sw);
			}

			//! 获得和另一个点距离的平方
			float32 GetDistanceFromSQ(const Vector4D& other) const
			{
				float32 sx = x - other.x;
				float32 sy = y - other.y;
				float32 sz = z - other.z;
				float32 sw = w - other.w;
				return sx*sx + sy*sy + sz*sz + sw*sw;
			}

			//! 本向量是否在给定的两个向量中间
			bool IsBetweenPoints(const Vector4D& begin, const Vector4D& end) const
			{
				float32 sx = end.x - begin.x;
				float32 sy = end.y - begin.y;
				float32 sz = end.z - begin.z;
				float32 sw = end.w - begin.w;
				const float32 f = sx*sx + sy*sy + sz*sz + sw*sw;
				return GetDistanceFromSQ(begin) <= f && GetDistanceFromSQ(end) <= f;
			}

			//! 向量归一化,浮点数有效
			Vector4D& Normalize()
			{
				float64 length = x*x + y*y + z*z + w*w;
				if (length == 0) return *this;

				length = 1.0 / sqrt(length);

				x = (float32)(x * length);
				y = (float32)(y * length);
				z = (float32)(z * length);
				w = (float32)(w * length);
				return *this;
			}

			//!	产生一个随机的单位向量,float32为浮点数时才有效
			Vector4D& RandUnit()
			{
				x = (float32)(random((float32)-1, (float32)1));
				y = (float32)(random((float32)-1, (float32)1));
				z = (float32)(random((float32)-1, (float32)1));
				w = (float32)(random((float32)-1, (float32)1));
				return Normalize();
			}

			//! 设置该向量的长度
			Vector4D& SetLength(float32 newlength)
			{
				Normalize();
				return (*this *= newlength);
			}

			//! 取该向量的相反向量
			Vector4D& Invert()
			{
				x *= -1;
				y *= -1;
				z *= -1;
				w *= -1;
				return *this;
			}

			//!本向量和给定的向量线性插值，返回一个新的向量
			Vector4D GetInterpolated(const Vector4D& other, float32 d) const
			{
				const float32 inv = 1.0f - d;
				return Vector4D
				(
					(other.x*inv + x*d), 
					(other.y*inv + y*d), 
					(other.z*inv + z*d), 
					(other.w*inv + w*d)
				);
			}

			//! 本向量和给定的两个向量二次插值，返回一个新的向量
			Vector4D GetInterpolatedQuadratic(const Vector4D& v2, const Vector4D& v3, float32 d) const
			{
				// this*(1-d)*(1-d) + 2 * v2 * (1-d) + v3 * d * d;
				const float32 inv = 1.0f - d;
				const float32 mul0 = inv * inv;
				const float32 mul1 = 2.0f * d * inv;
				const float32 mul2 = d * d;

				return Vector4D
				(
					(x * mul0 + v2.x * mul1 + v3.x * mul2),
					(y * mul0 + v2.y * mul1 + v3.y * mul2),
					(z * mul0 + v2.z * mul1 + v3.z * mul2),
					(w * mul0 + v2.w * mul1 + v3.w * mul2)
				);
			}

			//! 插值，使用给定的两个向量线性插值，返回修改过的本向量
			Vector4D& Interpolate(const Vector4D& a, const Vector4D& b, float32 d)
			{
				x = a.x + (b.x - a.x) * d;
				y = a.y + (b.y - a.y) * d;
				z = a.z + (b.z - a.z) * d;
				w = a.w + (b.w - a.w) * d;
				return *this;
			}

			//检测是否为空
			bool IsEmpty() const { return (iszero(x) && iszero(y) && iszero(z) && iszero(w)); }

			//清空
			void Clear() { x = y = z = w = 0; }

		public:
			float32 x;
			float32 y;
			float32 z;
			float32 w;
		};
	}

	typedef Math::Vector4D Vector4Df;
}
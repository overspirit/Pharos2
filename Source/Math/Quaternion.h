#pragma once

namespace Pharos
{
	namespace Math
	{
		// 四元数
		class CQuaternion
		{
		public:
			//! 默认构造函数
			CQuaternion()
			{
				x = 0.0f;
				y = 0.0f;
				z = 0.0f;
				w = 1.0f;
			}

			//! 用4个值初始化四元数
			CQuaternion(float32 x, float32 y, float32 z, float32 w)
			{
				this->x = x;
				this->y = y;
				this->z = z;
				this->w = w;
			}

			//! 用三个欧拉角初始化四元数
			CQuaternion(float32 x, float32 y, float32 z);

			//! 用欧拉角初始化四元数
			CQuaternion(const Vector3Df& vec);

			//! 用旋转矩阵初始化四元数
			CQuaternion(const Matrix4& mat);

			//! 等于运算符重载
			bool operator==(const CQuaternion& other) const;

			//! 不等于运算符重载
			bool operator!=(const CQuaternion& other) const;

			//! 赋值运算符重载
			inline CQuaternion& operator=(const CQuaternion& other);

			//! 赋值运算符重载
			inline CQuaternion& operator=(const Matrix4& other);

			//! 加运算符重载
			CQuaternion operator+(const CQuaternion& other) const;

			//! 乘运算符重载
			CQuaternion operator*(const CQuaternion& other) const;

			//! 乘运算符重载
			CQuaternion operator*(float32 s) const;

			//! 乘运算符重载
			CQuaternion& operator*=(float32 s);

			//! 乘运算符重载
			Vector3Df operator*(const Vector3Df& v) const;

			//! 乘运算符重载
			CQuaternion& operator*=(const CQuaternion& other);

			operator float32* () { return &x; }
			operator const float32* () const { return &x; }

			//! 和另一个四元数计算叉积,已验证
			inline float32 DotProduct(const CQuaternion& other) const;

			//! 用四个新值设置四元数,已验证
			inline CQuaternion& Set(float32 x, float32 y, float32 z, float32 w);

			//! 用三个欧拉角设置四元数,已验证
			inline CQuaternion& Set(float32 x, float32 y, float32 z);

			//! 用欧拉角设置四元数,已验证
			inline CQuaternion& Set(const Vector3Df& vec);

			//! 用另一个四元数设置四元数,已验证
			inline CQuaternion& Set(const CQuaternion& quat);

			//! 和另一个四元数比较,已验证
			inline bool Equals(const CQuaternion& other, const float32 tolerance = ROUNDING_ERROR_f32) const;

			//! 归一化四元数,已验证
			inline CQuaternion& Normalize();

			//! 从四元数得到旋转矩阵,已验证
			Matrix4 GetMatrix() const;

			//! 从四元数和给定的平移点建立一个新的矩阵，这个矩阵包含四元数的旋转操作和给定的平移操作,已验证
			void GetMatrix(Matrix4& dest, const Vector3Df& translation = Vector3Df()) const;

			//! 从四元数建立一个以给定的中心点旋转，给定的平移点移动的矩阵,已验证
			void GetMatrixCenter(Matrix4& dest, const Vector3Df& center, const Vector3Df& translation) const;

			//! 从四元数建立一个转置矩阵,已验证
			inline void GetMatrixTransposed(Matrix4 &dest) const;

			//! 求逆四元数,已验证
			CQuaternion& MakeInverse();

			//! 两个四元数插值,已验证
			CQuaternion& Lerp(const CQuaternion& q1, const CQuaternion& q2, float32 time);

			//! 两个四元数球面插值,已验证
			CQuaternion& Slerp(const CQuaternion& q1, const CQuaternion& q2, float32 time, float32 threshold = 0.05f);

			//! 以给定的向量为旋转轴，建立旋转angle度的四元数，给定的向量必须是单位向量,已验证
			CQuaternion& FromAngleAxis(float32 angle, const Vector3Df& axis);

			//! 从四元数中得到旋转轴和旋转角度,已验证
			void ToAngleAxis(float32& angle, Vector3Df& axis) const;

			//! 获得四元数旋转的三个欧拉角,已验证
			void ToEuler(Vector3Df& euler) const;

			//! 单位化四元数,已验证
			CQuaternion& MakeIdentity();

			//! 建立一个从from到to向量的旋转四元数,已验证
			CQuaternion& RotationFromTo(const Vector3Df& from, const Vector3Df& to);

			//! CQuaternion elements.
			float32 x; // vectorial (imaginary) part
			float32 y;
			float32 z;
			float32 w; // real part
		};


		// Constructor which converts euler angles to a CQuaternion
		inline CQuaternion::CQuaternion(float32 x, float32 y, float32 z)
		{
			Set(x, y, z);
		}

		// Constructor which converts euler angles to a CQuaternion
		inline CQuaternion::CQuaternion(const Vector3Df& vec)
		{
			Set(vec.x, vec.y, vec.z);
		}

		// Constructor which converts a matrix to a CQuaternion
		inline CQuaternion::CQuaternion(const Matrix4& mat)
		{
			(*this) = mat;
		}

		// equal operator
		inline bool CQuaternion::operator==(const CQuaternion& other) const
		{
			return (equals(x, other.x) && equals(y, other.y) && equals(z, other.z) && equals(w, other.w));
		}

		// inequality operator
		inline bool CQuaternion::operator!=(const CQuaternion& other) const
		{
			return !(*this == other);
		}

		// assignment operator
		inline CQuaternion& CQuaternion::operator=(const CQuaternion& other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
			return *this;
		}

		// matrix assignment operator
		inline CQuaternion& CQuaternion::operator=(const Matrix4& other)
		{
			const float32* m = other.m;

			const float32 diag = m[0] + m[5] + m[10] + 1;

			if (diag > 0.0f)
			{
				const float32 scale = sqrtf(diag) * 2.0f; // get scale from diagonal

				// TODO: speed this up
				x = (m[6] - m[9]) / scale;
				y = (m[8] - m[2]) / scale;
				z = (m[1] - m[4]) / scale;
				w = 0.25f * scale;
			}
			else
			{
				if (m[0] > m[5] && m[0] > m[10])
				{
					// 1st element of diag is greatest value
					// find scale according to 1st element, and double it
					const float32 scale = sqrtf(1.0f + m[0] - m[5] - m[10]) * 2.0f;

					// TODO: speed this up
					x = 0.25f * scale;
					y = (m[4] + m[1]) / scale;
					z = (m[2] + m[8]) / scale;
					w = (m[6] - m[9]) / scale;
				}
				else if (m[5] > m[10])
				{
					// 2nd element of diag is greatest value
					// find scale according to 2nd element, and double it
					const float32 scale = sqrtf(1.0f + m[5] - m[0] - m[10]) * 2.0f;

					// TODO: speed this up
					x = (m[4] + m[1]) / scale;
					y = 0.25f * scale;
					z = (m[9] + m[6]) / scale;
					w = (m[8] - m[2]) / scale;
				}
				else
				{
					// 3rd element of diag is greatest value
					// find scale according to 3rd element, and double it
					const float32 scale = sqrtf(1.0f + m[10] - m[0] - m[5]) * 2.0f;

					// TODO: speed this up
					x = (m[8] + m[2]) / scale;
					y = (m[9] + m[6]) / scale;
					z = 0.25f * scale;
					w = (m[1] - m[4]) / scale;
				}
			}

			return Normalize();
		}

		// add operator
		inline CQuaternion CQuaternion::operator+(const CQuaternion& other) const
		{
			return CQuaternion(x + other.x, y + other.y, z + other.z, w + other.w);
		}

		// multiplication operator
		inline CQuaternion CQuaternion::operator*(const CQuaternion& other) const
		{
			CQuaternion tmp;

			tmp.w = (other.w * w) - (other.x * x) - (other.y * y) - (other.z * z);
			tmp.x = (other.w * x) + (other.x * w) + (other.y * z) - (other.z * y);
			tmp.y = (other.w * y) + (other.y * w) + (other.z * x) - (other.x * z);
			tmp.z = (other.w * z) + (other.z * w) + (other.x * y) - (other.y * x);

			return tmp;
		}


		// multiplication operator
		inline CQuaternion CQuaternion::operator*(float32 s) const
		{
			return CQuaternion(s*x, s*y, s*z, s*w);
		}


		// multiplication operator
		inline CQuaternion& CQuaternion::operator*=(float32 s)
		{
			x *= s;
			y *= s;
			z *= s;
			w *= s;
			return *this;
		}

		inline Vector3Df CQuaternion::operator* (const Vector3Df& v) const
		{
			// nVidia SDK implementation

			Vector3Df uv, uuv;
			Vector3Df qvec(x, y, z);
			uv = qvec.CrossProduct(v);
			uuv = qvec.CrossProduct(uv);
			uv *= (2.0f * w);
			uuv *= 2.0f;

			return v + uv + uuv;
		}

		// multiplication operator
		inline CQuaternion& CQuaternion::operator*=(const CQuaternion& other)
		{
			return (*this = other * (*this));
		}

		// calculates the dot product
		inline float32 CQuaternion::DotProduct(const CQuaternion& q2) const
		{
			return (x * q2.x) + (y * q2.y) + (z * q2.z) + (w * q2.w);
		}

		// sets new CQuaternion
		inline CQuaternion& CQuaternion::Set(float32 x, float32 y, float32 z, float32 w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
			return *this;
		}

		// sets new CQuaternion based on euler angles
		inline CQuaternion& CQuaternion::Set(float32 x, float32 y, float32 z)
		{
			float64 angle;

			angle = x * 0.5;
			const float64 sr = sin(angle);
			const float64 cr = cos(angle);

			angle = y * 0.5;
			const float64 sp = sin(angle);
			const float64 cp = cos(angle);

			angle = z * 0.5;
			const float64 sy = sin(angle);
			const float64 cy = cos(angle);

			const float64 cpcy = cp * cy;
			const float64 spcy = sp * cy;
			const float64 cpsy = cp * sy;
			const float64 spsy = sp * sy;

			this->x = (float32)(sr * cpcy - cr * spsy);
			this->y = (float32)(cr * spcy + sr * cpsy);
			this->z = (float32)(cr * cpsy - sr * spcy);
			this->w = (float32)(cr * cpcy + sr * spsy);

			return Normalize();
		}

		// sets new CQuaternion based on euler angles
		inline CQuaternion& CQuaternion::Set(const Vector3Df& vec)
		{
			return Set(vec.x, vec.y, vec.z);
		}

		// sets new CQuaternion based on other CQuaternion
		inline CQuaternion& CQuaternion::Set(const CQuaternion& quat)
		{
			return (*this = quat);
		}

		//! returns if this CQuaternion equals the other one, taking floating point rounding errors into account
		inline bool CQuaternion::Equals(const CQuaternion& other, const float32 tolerance) const
		{
			return equals(x, other.x, tolerance) && equals(y, other.y, tolerance) &&
				equals(z, other.z, tolerance) && equals(w, other.w, tolerance);
		}

		// normalizes the CQuaternion
		inline CQuaternion& CQuaternion::Normalize()
		{
			const float32 n = x*x + y*y + z*z + w*w;

			if (n == 1)
				return *this;

			//n = 1.0f / sqrtf(n);
			return (*this *= (float32)(1.0 / sqrt(n)));
		}

		// Creates a matrix from this CQuaternion
		inline Matrix4 CQuaternion::GetMatrix() const
		{
			Matrix4 m;
			GetMatrix(m);
			return m;
		}

		/*!
		Creates a matrix from this CQuaternion
		*/
		inline void CQuaternion::GetMatrix(Matrix4& dest, const Vector3Df& translation) const
		{
			dest.m[0] = 1.0f - 2.0f*y*y - 2.0f*z*z;
			dest.m[1] = 2.0f*x*y + 2.0f*z*w;
			dest.m[2] = 2.0f*x*z - 2.0f*y*w;
			dest.m[3] = 0.0f;

			dest.m[4] = 2.0f*x*y - 2.0f*z*w;
			dest.m[5] = 1.0f - 2.0f*x*x - 2.0f*z*z;
			dest.m[6] = 2.0f*z*y + 2.0f*x*w;
			dest.m[7] = 0.0f;

			dest.m[8] = 2.0f*x*z + 2.0f*y*w;
			dest.m[9] = 2.0f*z*y - 2.0f*x*w;
			dest.m[10] = 1.0f - 2.0f*x*x - 2.0f*y*y;
			dest.m[11] = 0.0f;

			dest.m[12] = translation.x;
			dest.m[13] = translation.y;
			dest.m[14] = translation.z;
			dest.m[15] = 1.f;
		}

		/*!
		Creates a matrix from this CQuaternion
		Rotate about a center point
		shortcut for
		CQuaternion q;
		q.rotationFromTo(vin[i].normal, forward);
		q.getMatrix(lookat, center);

		Matrix4 m2;
		m2.setInverseTranslation(center);
		lookat *= m2;
		*/
		inline void CQuaternion::GetMatrixCenter(Matrix4 &dest, const Vector3Df &center, const Vector3Df &translation) const
		{
			dest.m[0] = 1.0f - 2.0f*y*y - 2.0f*z*z;
			dest.m[1] = 2.0f*x*y + 2.0f*z*w;
			dest.m[2] = 2.0f*x*z - 2.0f*y*w;
			dest.m[3] = 0.0f;

			dest.m[4] = 2.0f*x*y - 2.0f*z*w;
			dest.m[5] = 1.0f - 2.0f*x*x - 2.0f*z*z;
			dest.m[6] = 2.0f*z*y + 2.0f*x*w;
			dest.m[7] = 0.0f;

			dest.m[8] = 2.0f*x*z + 2.0f*y*w;
			dest.m[9] = 2.0f*z*y - 2.0f*x*w;
			dest.m[10] = 1.0f - 2.0f*x*x - 2.0f*y*y;
			dest.m[11] = 0.0f;

			dest.m[12] = -dest.m[0] * center.x - dest.m[4] * center.y - dest.m[8] * center.z + (center.x - translation.x);
			dest.m[13] = -dest.m[1] * center.x - dest.m[5] * center.y - dest.m[9] * center.z + (center.y - translation.y);
			dest.m[14] = -dest.m[2] * center.x - dest.m[6] * center.y - dest.m[10] * center.z + (center.z - translation.z);
			dest.m[15] = 1.0f;
		}

		// Creates a matrix from this CQuaternion
		inline void CQuaternion::GetMatrixTransposed(Matrix4 &dest) const
		{
			dest.m[0] = 1.0f - 2.0f*y*y - 2.0f*z*z;
			dest.m[4] = 2.0f*x*y + 2.0f*z*w;
			dest.m[8] = 2.0f*x*z - 2.0f*y*w;
			dest.m[12] = 0.0f;

			dest.m[1] = 2.0f*x*y - 2.0f*z*w;
			dest.m[5] = 1.0f - 2.0f*x*x - 2.0f*z*z;
			dest.m[9] = 2.0f*z*y + 2.0f*x*w;
			dest.m[13] = 0.0f;

			dest.m[2] = 2.0f*x*z + 2.0f*y*w;
			dest.m[6] = 2.0f*z*y - 2.0f*x*w;
			dest.m[10] = 1.0f - 2.0f*x*x - 2.0f*y*y;
			dest.m[14] = 0.0f;

			dest.m[3] = 0.f;
			dest.m[7] = 0.f;
			dest.m[11] = 0.f;
			dest.m[15] = 1.f;

			//dest.SetDefinitelyIdentityMatrix(false);
		}

		// Inverts this CQuaternion
		inline CQuaternion& CQuaternion::MakeInverse()
		{
			x = -x; y = -y; z = -z;
			return *this;
		}

		// set this CQuaternion to the result of the linear interpolation between two quaternions
		inline CQuaternion& CQuaternion::Lerp(const CQuaternion& q1, const CQuaternion& q2, float32 time)
		{
			const float32 scale = 1.0f - time;
			return (*this = (q1*scale) + (q2*time));
		}


		// set this CQuaternion to the result of the interpolation between two quaternions
		inline CQuaternion& CQuaternion::Slerp(const CQuaternion& q1, const CQuaternion& q2, float32 time, float32 threshold)
		{
			float32 angle = q1.DotProduct(q2);

			CQuaternion temp = q1;

			// make sure we use the short rotation
			if (angle < 0.0f)
			{
				temp = q1 * -1.0f;
				angle *= -1.0f;
			}

			if (angle <= (1 - threshold)) // spherical interpolation
			{
				const float32 theta = acosf(angle);
				const float32 invsintheta = 1.0f / sinf(theta);
				const float32 scale = sinf(theta * (1.0f - time)) * invsintheta;
				const float32 invscale = sinf(theta * time) * invsintheta;
				return (*this = (temp * scale) + (q2 * invscale));
			}
			else // linear interploation
				return Lerp(temp, q2, time);
		}

		//! axis must be unit length, angle in radians
		inline CQuaternion& CQuaternion::FromAngleAxis(float32 angle, const Vector3Df& axis)
		{
			const float32 fHalfAngle = 0.5f*angle;
			const float32 fSin = sinf(fHalfAngle);
			w = cosf(fHalfAngle);
			x = fSin*axis.x;
			y = fSin*axis.y;
			z = fSin*axis.z;
			return *this;
		}


		inline void CQuaternion::ToAngleAxis(float32& angle, Vector3Df& axis) const
		{
			const float32 scale = sqrtf(x*x + y*y + z*z);

			if (iszero(scale) || w > 1.0f || w < -1.0f)
			{
				angle = 0.0f;
				axis.x = 0.0f;
				axis.y = 1.0f;
				axis.z = 0.0f;
			}
			else
			{
				const float32 invscale = 1.0f / scale;
				angle = 2.0f * acosf(w);
				axis.x = x * invscale;
				axis.y = y * invscale;
				axis.z = z * invscale;
			}
		}

		inline void CQuaternion::ToEuler(Vector3Df& euler) const
		{
			const float64 sqw = w*w;
			const float64 sqx = x*x;
			const float64 sqy = y*y;
			const float64 sqz = z*z;
			const float64 test = 2.0 * (y*w - x*z);

			if (equals(test, 1.0, 0.000001))
			{
				// heading = rotation about z-axis
				euler.z = (float32)(-2.0*atan2(x, w));
				// bank = rotation about x-axis
				euler.x = 0;
				// attitude = rotation about y-axis
				euler.y = (float32)(PI / 2.0);
			}
			else if (equals(test, -1.0, 0.000001))
			{
				// heading = rotation about z-axis
				euler.z = (float32)(2.0*atan2(x, w));
				// bank = rotation about x-axis
				euler.x = 0;
				// attitude = rotation about y-axis
				euler.y = (float32)(PI / -2.0);
			}
			else
			{
				// heading = rotation about z-axis
				euler.z = (float32)atan2(2.0 * (x*y + z*w), (sqx - sqy - sqz + sqw));
				// bank = rotation about x-axis
				euler.x = (float32)atan2(2.0 * (y*z + x*w), (-sqx - sqy + sqz + sqw));
				// attitude = rotation about y-axis
				euler.y = (float32)asin(Math::clamp(test, -1.0, 1.0));
			}
		}

		// set CQuaternion to identity
		inline CQuaternion& CQuaternion::MakeIdentity()
		{
			w = 1.f;
			x = 0.f;
			y = 0.f;
			z = 0.f;
			return *this;
		}

		inline CQuaternion& CQuaternion::RotationFromTo(const Vector3Df& from, const Vector3Df& to)
		{
			// Based on Stan Melax's article in Game Programming Gems
			// Copy, since cannot modify local
			Vector3Df v0 = from;
			Vector3Df v1 = to;
			v0.Normalize();
			v1.Normalize();

			const float32 d = v0.DotProduct(v1);
			if (d >= 1.0f) // If dot == 1, vectors are the same
			{
				return MakeIdentity();
			}
			else if (d <= -1.0f) // exactly opposite
			{
				Vector3Df axis(1.0f, 0.f, 0.f);
				axis = axis.CrossProduct(v0);
				if (axis.GetLength() == 0)
				{
					axis.Set(0.f, 1.f, 0.f);
					axis.CrossProduct(v0);
				}
				// same as fromAngleAxis(PI, axis).normalize();
				return Set(axis.x, axis.y, axis.z, 0).Normalize();
			}

			const float32 s = sqrtf((1 + d) * 2); // optimize inv_sqrt
			const float32 invs = 1.f / s;
			const Vector3Df c = v0.CrossProduct(v1)*invs;
			return Set(c.x, c.y, c.z, s * 0.5f).Normalize();
		}

		//! 获得旋转矩阵的四元数，已验证
		inline CQuaternion Matrix4::GetQuaternion() const
		{
			CQuaternion quat;

			float32 s;
			float32 tq[4];
			int32    i, j;

			// Use tq to store the largest trace
			tq[0] = 1 + m[0] + m[5] + m[10];
			tq[1] = 1 + m[0] - m[5] - m[10];
			tq[2] = 1 - m[0] + m[5] - m[10];
			tq[3] = 1 - m[0] - m[5] + m[10];

			// Find the maximum (could also use stacked if's later)
			j = 0;
			for (i = 1; i<4; i++) j = (tq[i]>tq[j]) ? i : j;

			// check the diagonal
			if (j == 0)
			{
				/* perform instant calculation */
				quat.w = tq[0];
				quat.x = m[6] - m[9];
				quat.y = m[8] - m[2];
				quat.z = m[1] - m[4];
			}
			else if (j == 1)
			{
				quat.w = m[6] - m[9];
				quat.x = tq[1];
				quat.y = m[1] + m[4];
				quat.z = m[8] + m[2];
			}
			else if (j == 2)
			{
				quat.w = m[8] - m[2];
				quat.x = m[1] + m[4];
				quat.y = tq[2];
				quat.z = m[6] + m[9];
			}
			else /* if (j==3) */
			{
				quat.w = m[1] - m[4];
				quat.x = m[8] + m[2];
				quat.y = m[6] + m[9];
				quat.z = tq[3];
			}

			s = sqrt(0.25f / tq[j]);
			quat.w *= s;
			quat.x *= s;
			quat.y *= s;
			quat.z *= s;

			return quat;
		}
	}

	typedef Math::CQuaternion Quaternion;
}
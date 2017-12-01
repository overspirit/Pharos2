#pragma once

namespace Pharos
{
	namespace Math
	{
		//双四元数
		class CDualQuaternion
		{
		public:
			CDualQuaternion()
			{
				m_real.Set(0, 0, 0, 1);
				m_dual.Set(0, 0, 0, 0);
			}

			CDualQuaternion(const CQuaternion& r, const CQuaternion& d)
			{
				m_real = r;
				m_dual = d;

				m_real.Normalize();
			}

			CDualQuaternion(const CQuaternion& r, const Vector3Df& t)
			{
				m_real = r;
				m_dual = CQuaternion(t.x, t.y, t.z, 0) * m_real * 0.5f;

				m_real.Normalize();
			}

			inline CDualQuaternion operator+ (const CDualQuaternion& other) const;

			inline CDualQuaternion operator* (const CDualQuaternion& other) const;

			inline CDualQuaternion operator* (float32 scale) const;

			//和另一个双四元数做叉积
			inline float32 DotProduct(const CDualQuaternion& other) const;

			//归一化
			inline CDualQuaternion& Normalize();

			//反转
			inline CDualQuaternion& MakeInverse();

			//从双四元数从取出旋转四元数
			inline CQuaternion GetRotation() const;

			//从双四元数从取出平移向量
			inline Vector3Df GetTranslation() const;

			//将双四元数转成矩阵
			inline Matrix4 ToMatrix() const;

		public:
			CQuaternion m_real;
			CQuaternion m_dual;
		};

		inline CDualQuaternion CDualQuaternion::operator+ (const CDualQuaternion& other) const
		{
			return CDualQuaternion(m_real + other.m_real, m_dual + other.m_dual);
		}

		inline CDualQuaternion CDualQuaternion::operator* (const CDualQuaternion& other) const
		{
			return CDualQuaternion(m_real * other.m_real, m_dual * other.m_real + m_real * other.m_dual);
		}

		inline CDualQuaternion CDualQuaternion::operator* (float32 scale) const
		{
			return CDualQuaternion(m_real * scale, m_dual * scale);
		}

		inline float32 CDualQuaternion::DotProduct(const CDualQuaternion& other) const
		{
			return m_real.DotProduct(other.m_real);
		}

		inline CDualQuaternion& CDualQuaternion::Normalize()
		{
			float mag = m_real.DotProduct(m_real);
			m_real *= 1.0f / mag;
			m_dual *= 1.0f / mag;
			return *this;
		}

		inline CDualQuaternion& CDualQuaternion::MakeInverse()
		{
			m_real.MakeInverse();
			m_dual.MakeInverse();
			return *this;
		}

		inline CQuaternion CDualQuaternion::GetRotation() const
		{
			return m_real;
		}

		inline Vector3Df CDualQuaternion::GetTranslation() const
		{
			CQuaternion t = (m_dual * 2.0f) * CQuaternion(-m_real.x, -m_real.y, -m_real.z, m_real.w);
			return Vector3Df(t.x, t.y, t.z);
		}

		inline Matrix4 CDualQuaternion::ToMatrix() const
		{
			Matrix4 mat;

			float32 len2 = m_real.DotProduct(m_real);
			float32 w = m_real.w / len2;
			float32 x = m_real.x / len2;
			float32 y = m_real.y / len2;
			float32 z = m_real.z / len2;
			float32 t0 = m_dual.w / len2;
			float32 t1 = m_dual.x / len2;
			float32 t2 = m_dual.y / len2;
			float32 t3 = m_dual.z / len2;

			mat.m[0] = w * w + x * x - y * y - z * z;
			mat.m[1] = 2 * x * y + 2 * w * z;
			mat.m[2] = 2 * x * z - 2 * w * y;
			mat.m[3] = 0;

			mat.m[4] = 2 * x * y - 2 * w * z;
			mat.m[5] = w * w + y * y - x * x - z * z;
			mat.m[6] = 2 * y * z + 2 * w * x;
			mat.m[7] = 0;

			mat.m[8] = 2 * x * z + 2 * w * y;
			mat.m[9] = 2 * y * z - 2 * w * x;
			mat.m[10] = w * w + z * z - x * x - y * y;
			mat.m[11] = 0;

			mat.m[12] = 2.0f * w * t1 - 2.0f * x * t0 - 2.0f * y * t3 + 2.0f * z * t2;
			mat.m[13] = 2.0f * w * t2 - 2.0f * y * t0 - 2.0f * z * t1 + 2.0f * x * t3;
			mat.m[14] = 2.0f * w * t3 - 2.0f * z * t0 - 2.0f * x * t2 + 2.0f * y * t1;
			mat.m[15] = len2;

			return mat;
		}
	}

	typedef Math::CDualQuaternion DualQuaternion;
}
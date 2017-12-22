#pragma once

namespace Pharos
{
	namespace Math
	{
		class CQuaternion;

		// 4x4矩阵，行主序
		class CMatrix4
		{
		public:
			//! 默认构造函数
			CMatrix4();

			//! 拷贝函数
			CMatrix4(const CMatrix4& other);

			//! 根据行索引和列索引获得矩阵的某个值
			float32& operator()(const int32 row, const int32 col) { return m[row * 4 + col]; }
			const float32& operator()(const int32 row, const int32 col) const { return m[row * 4 + col]; }

			//! 运算符重载
			inline CMatrix4& operator=(const CMatrix4& other);
			inline CMatrix4& operator=(float32 scalar);

			bool operator==(const CMatrix4& other) const;
			bool operator!=(const CMatrix4& other) const;

			CMatrix4 operator+(const CMatrix4& other) const;
			CMatrix4& operator+=(const CMatrix4& other);
			CMatrix4 operator-(const CMatrix4& other) const;
			CMatrix4& operator-=(const CMatrix4& other);
			CMatrix4 operator*(const CMatrix4& other) const;
			CMatrix4& operator*=(const CMatrix4& other);
			CMatrix4 operator*(float32 scalar) const;
			CMatrix4& operator*=(float32 scalar);

			operator float32* () { return m; }
			operator const float32* () const { return m; }

			//! 设置矩阵所有的值，已验证
			CMatrix4& SetData(const float32* data);

			//! 计算两个矩阵相乘
			CMatrix4& SetByProduct(const CMatrix4& other_a, const CMatrix4& other_b);

			//! 将矩阵初始化成单位矩阵,已验证
			inline CMatrix4& MakeIdentity();

			//! 矩阵是否为单位矩阵,已验证
			inline bool IsIdentity() const;

			//! 矩阵是否为正交矩阵，未验证
			inline bool IsOrthogonal() const;

			//! 计算逆矩阵,已验证
			bool MakeInverse();

			//! 获得逆矩阵,已验证
			bool GetInverse(CMatrix4& out) const;
			
			//! 获得本矩阵的转置矩阵,已验证
			CMatrix4 GetTransposed() const;

			//! 获得本矩阵的转置矩阵,已验证
			inline void GetTransposed(CMatrix4& dest) const;

			//! 获得旋转矩阵的四元数，已验证
			CQuaternion GetQuaternion() const;

			//! 根据给定的点设置矩阵的平移值，已验证
			CMatrix4& SetTranslation(const Vector3Df& translation);

			//! 获得矩阵的平移值，已验证
			Vector3Df GetTranslation() const;

			//! 根据给定的3个欧拉角创建旋转矩阵，这个是弧度制，未验证
			inline CMatrix4& SetRotationRadians(const Vector3Df& rotation);

			//! 根据给定的3个欧拉角创建旋转矩阵，这个是角度制，未验证
			CMatrix4& SetRotationDegrees(const Vector3Df& rotation);

			//! 根据给定的3个欧拉角创建反方向的旋转矩阵，这个是弧度制，未验证
			inline CMatrix4& SetInverseRotationRadians(const Vector3Df& rotation);

			//! 根据给定的3个欧拉角创建反方向的旋转矩阵，这个是角度制，未验证
			inline CMatrix4& SetInverseRotationDegrees(const Vector3Df& rotation);

			//! 以给定的向量为旋转轴，创建旋转angle度的旋转矩阵，angle为弧度制，已验证
			inline CMatrix4& SetRotationAxisRadians(float32 angle, const Vector3Df& axis);
			
			//! 根据给定的向量创建缩放矩阵，已验证
			CMatrix4& SetScale(const Vector3Df& scale);

			//! 创建一个三个方向相同缩放比例的缩放矩阵，已验证
			CMatrix4& SetScale(float32 scale) { return SetScale(Vector3Df(scale, scale, scale)); }
			
			//! 获得缩放比例，未验证
			Vector3Df GetScale() const;

			//! 用矩阵转换给定的点，已验证
			Vector4D Transform(const Vector2D& vect) const;
			Vector4D Transform(const Vector3Df& vect) const;
			Vector4D Transform(const Vector4D& vect) const;
			
			//! 用矩阵转换给定的点，已验证
			Vector2D TransformCoord(const Vector2D& vect) const;
			Vector3Df TransformCoord(const Vector3Df& vect) const;

			//! 用矩阵转换给定的向量，已验证
			Vector2D TransformNormal(const Vector2D& vect) const;
			Vector3Df TransformNormal(const Vector3Df& vect) const;

			//! 使用FOV建立一个右手坐标系的透视投影矩阵,已验证
			CMatrix4& BuildProjectionMatrixPerspectiveFovRH(float32 fieldOfViewRadians, float32 aspectRatio, float32 zNear, float32 zFar);

			//! 使用FOV建立一个左手坐标系的透视投影矩阵,已验证
			CMatrix4& BuildProjectionMatrixPerspectiveFovLH(float32 fieldOfViewRadians, float32 aspectRatio, float32 zNear, float32 zFar);

			//! 使用FOV建立一个Z无限远的左手坐标系的透视投影矩阵，未验证
			CMatrix4& BuildProjectionMatrixPerspectiveFovInfinityLH(float32 fieldOfViewRadians, float32 aspectRatio, float32 zNear, float32 epsilon = 0);

			//! 使用宽高建立一个右手坐标系的透视投影矩阵,已验证
			CMatrix4& BuildProjectionMatrixPerspectiveRH(float32 widthOfViewVolume, float32 heightOfViewVolume, float32 zNear, float32 zFar);

			//! 使用宽高建立一个左手坐标系的透视投影矩阵,已验证
			CMatrix4& BuildProjectionMatrixPerspectiveLH(float32 widthOfViewVolume, float32 heightOfViewVolume, float32 zNear, float32 zFar);

			//! 使用宽高建立一个左手坐标系的正交投影矩阵,已验证
			CMatrix4& BuildProjectionMatrixOrthoLH(float32 widthOfViewVolume, float32 heightOfViewVolume, float32 zNear, float32 zFar);

			//! 使用宽高建立一个右手坐标系的正交投影矩阵,已验证
			CMatrix4& BuildProjectionMatrixOrthoRH(float32 widthOfViewVolume, float32 heightOfViewVolume, float32 zNear, float32 zFar);

			//! 建立一个左手坐标系的观察矩阵,已验证
			CMatrix4& BuildCameraLookAtMatrixLH(const Vector3Df& position, const Vector3Df& target, const Vector3Df& upVector);

			//! 建立一个右手坐标系的观察矩阵,已验证
			CMatrix4& BuildCameraLookAtMatrixRH(const Vector3Df& position, const Vector3Df& target, const Vector3Df& upVector);

			//! 建立一个阴影矩阵，point为0时光源为点光源，point为1时光源为方向光,未验证
			CMatrix4& BuildShadowMatrix(const Vector3Df& light, Plane3Df plane, float32 point = 1.0f);
			
			//! 建立一个from向量到to向量的旋转矩阵，已验证
			CMatrix4& BuildRotateFromTo(const Vector3Df& from, const Vector3Df& to);

			//! 和另一个矩阵插值，time必须为0至1之间，已验证
			CMatrix4 Interpolate(const CMatrix4& b, float32 time) const;

			//! 比较，已验证
			bool Equals(const CMatrix4& other, const float32 tolerance = ROUNDING_ERROR_f32) const;

		public:
			//! Matrix data, stored in row-major order
			float32 m[16];
		};


		// Default constructor
		inline CMatrix4::CMatrix4()
		{
			MakeIdentity();
		}

		// Copy constructor
		inline CMatrix4::CMatrix4(const CMatrix4& other)
		{
			*this = other;
		}

		inline CMatrix4& CMatrix4::operator=(const CMatrix4 &other)
		{
			memcpy(m, other.m, 16 * sizeof(float32));
			return *this;
		}

		inline CMatrix4& CMatrix4::operator=(float32 scalar)
		{
			for (int32 i = 0; i < 16; i++)
				m[i] = scalar;
			return *this;
		}

		inline bool CMatrix4::operator==(const CMatrix4 &other) const
		{
			for (int32 i = 0; i < 16; ++i)
			{
				if (m[i] != other.m[i])
					return false;
			}

			return true;
		}

		inline bool CMatrix4::operator!=(const CMatrix4 &other) const
		{
			return !(*this == other);
		}

		//! Add another matrix.
		inline CMatrix4 CMatrix4::operator+(const CMatrix4& other) const
		{
			CMatrix4 temp;

			temp.m[0] = m[0] + other.m[0];
			temp.m[1] = m[1] + other.m[1];
			temp.m[2] = m[2] + other.m[2];
			temp.m[3] = m[3] + other.m[3];
			temp.m[4] = m[4] + other.m[4];
			temp.m[5] = m[5] + other.m[5];
			temp.m[6] = m[6] + other.m[6];
			temp.m[7] = m[7] + other.m[7];
			temp.m[8] = m[8] + other.m[8];
			temp.m[9] = m[9] + other.m[9];
			temp.m[10] = m[10] + other.m[10];
			temp.m[11] = m[11] + other.m[11];
			temp.m[12] = m[12] + other.m[12];
			temp.m[13] = m[13] + other.m[13];
			temp.m[14] = m[14] + other.m[14];
			temp.m[15] = m[15] + other.m[15];

			return temp;
		}

		//! Add another matrix.
		inline CMatrix4& CMatrix4::operator+=(const CMatrix4& other)
		{
			m[0] += other.m[0];
			m[1] += other.m[1];
			m[2] += other.m[2];
			m[3] += other.m[3];
			m[4] += other.m[4];
			m[5] += other.m[5];
			m[6] += other.m[6];
			m[7] += other.m[7];
			m[8] += other.m[8];
			m[9] += other.m[9];
			m[10] += other.m[10];
			m[11] += other.m[11];
			m[12] += other.m[12];
			m[13] += other.m[13];
			m[14] += other.m[14];
			m[15] += other.m[15];

			return *this;
		}

		//! Subtract another matrix.
		inline CMatrix4 CMatrix4::operator-(const CMatrix4& other) const
		{
			CMatrix4 temp;

			temp.m[0] = m[0] - other.m[0];
			temp.m[1] = m[1] - other.m[1];
			temp.m[2] = m[2] - other.m[2];
			temp.m[3] = m[3] - other.m[3];
			temp.m[4] = m[4] - other.m[4];
			temp.m[5] = m[5] - other.m[5];
			temp.m[6] = m[6] - other.m[6];
			temp.m[7] = m[7] - other.m[7];
			temp.m[8] = m[8] - other.m[8];
			temp.m[9] = m[9] - other.m[9];
			temp.m[10] = m[10] - other.m[10];
			temp.m[11] = m[11] - other.m[11];
			temp.m[12] = m[12] - other.m[12];
			temp.m[13] = m[13] - other.m[13];
			temp.m[14] = m[14] - other.m[14];
			temp.m[15] = m[15] - other.m[15];

			return temp;
		}

		//! Subtract another matrix.
		inline CMatrix4& CMatrix4::operator-=(const CMatrix4& other)
		{
			m[0] -= other.m[0];
			m[1] -= other.m[1];
			m[2] -= other.m[2];
			m[3] -= other.m[3];
			m[4] -= other.m[4];
			m[5] -= other.m[5];
			m[6] -= other.m[6];
			m[7] -= other.m[7];
			m[8] -= other.m[8];
			m[9] -= other.m[9];
			m[10] -= other.m[10];
			m[11] -= other.m[11];
			m[12] -= other.m[12];
			m[13] -= other.m[13];
			m[14] -= other.m[14];
			m[15] -= other.m[15];

			return *this;
		}

		//! multiply by another matrix
		inline CMatrix4 CMatrix4::operator*(const CMatrix4& other) const
		{
			CMatrix4 mat;

			const float32* m1 = m;
			const float32* m2 = other.m;

			mat.m[0] = m1[0] * m2[0] + m1[1] * m2[4] + m1[2] * m2[8] + m1[3] * m2[12];
			mat.m[1] = m1[0] * m2[1] + m1[1] * m2[5] + m1[2] * m2[9] + m1[3] * m2[13];
			mat.m[2] = m1[0] * m2[2] + m1[1] * m2[6] + m1[2] * m2[10] + m1[3] * m2[14];
			mat.m[3] = m1[0] * m2[3] + m1[1] * m2[7] + m1[2] * m2[11] + m1[3] * m2[15];

			mat.m[4] = m1[4] * m2[0] + m1[5] * m2[4] + m1[6] * m2[8] + m1[7] * m2[12];
			mat.m[5] = m1[4] * m2[1] + m1[5] * m2[5] + m1[6] * m2[9] + m1[7] * m2[13];
			mat.m[6] = m1[4] * m2[2] + m1[5] * m2[6] + m1[6] * m2[10] + m1[7] * m2[14];
			mat.m[7] = m1[4] * m2[3] + m1[5] * m2[7] + m1[6] * m2[11] + m1[7] * m2[15];

			mat.m[8] = m1[8] * m2[0] + m1[9] * m2[4] + m1[10] * m2[8] + m1[11] * m2[12];
			mat.m[9] = m1[8] * m2[1] + m1[9] * m2[5] + m1[10] * m2[9] + m1[11] * m2[13];
			mat.m[10] = m1[8] * m2[2] + m1[9] * m2[6] + m1[10] * m2[10] + m1[11] * m2[14];
			mat.m[11] = m1[8] * m2[3] + m1[9] * m2[7] + m1[10] * m2[11] + m1[11] * m2[15];

			mat.m[12] = m1[12] * m2[0] + m1[13] * m2[4] + m1[14] * m2[8] + m1[15] * m2[12];
			mat.m[13] = m1[12] * m2[1] + m1[13] * m2[5] + m1[14] * m2[9] + m1[15] * m2[13];
			mat.m[14] = m1[12] * m2[2] + m1[13] * m2[6] + m1[14] * m2[10] + m1[15] * m2[14];
			mat.m[15] = m1[12] * m2[3] + m1[13] * m2[7] + m1[14] * m2[11] + m1[15] * m2[15];

			return mat;
		}

		//! Multiply by another matrix.
		inline CMatrix4& CMatrix4::operator*=(const CMatrix4& other)
		{
			CMatrix4 temp(*this);
			return SetByProduct(temp, other);
		}

		//! Multiply by scalar.
		inline CMatrix4 CMatrix4::operator*(float32 scalar) const
		{
			CMatrix4 temp;

			temp.m[0] = m[0] * scalar;
			temp.m[1] = m[1] * scalar;
			temp.m[2] = m[2] * scalar;
			temp.m[3] = m[3] * scalar;
			temp.m[4] = m[4] * scalar;
			temp.m[5] = m[5] * scalar;
			temp.m[6] = m[6] * scalar;
			temp.m[7] = m[7] * scalar;
			temp.m[8] = m[8] * scalar;
			temp.m[9] = m[9] * scalar;
			temp.m[10] = m[10] * scalar;
			temp.m[11] = m[11] * scalar;
			temp.m[12] = m[12] * scalar;
			temp.m[13] = m[13] * scalar;
			temp.m[14] = m[14] * scalar;
			temp.m[15] = m[15] * scalar;

			return temp;
		}

		//! Multiply by scalar.
		inline CMatrix4& CMatrix4::operator*=(float32 scalar)
		{
			m[0] *= scalar;
			m[1] *= scalar;
			m[2] *= scalar;
			m[3] *= scalar;
			m[4] *= scalar;
			m[5] *= scalar;
			m[6] *= scalar;
			m[7] *= scalar;
			m[8] *= scalar;
			m[9] *= scalar;
			m[10] *= scalar;
			m[11] *= scalar;
			m[12] *= scalar;
			m[13] *= scalar;
			m[14] *= scalar;
			m[15] *= scalar;

			return *this;
		}

		//! 设置矩阵所有的值，已验证
		// sets all matrix data members at once
		inline CMatrix4& CMatrix4::SetData(const float32* data)
		{
			memcpy(m, data, 16 * sizeof(float32));

			return *this;
		}

		//! multiply by another matrix
		// set this matrix to the product of two other matrices
		// goal is to reduce stack use and copy
		//! 计算两个矩阵相乘
		inline CMatrix4& CMatrix4::SetByProduct(const CMatrix4& other_a, const CMatrix4& other_b)
		{
			const float32* m1 = other_a.m;
			const float32* m2 = other_b.m;

			m[0] = m1[0] * m2[0] + m1[1] * m2[4] + m1[2] * m2[8] + m1[3] * m2[12];
			m[1] = m1[0] * m2[1] + m1[1] * m2[5] + m1[2] * m2[9] + m1[3] * m2[13];
			m[2] = m1[0] * m2[2] + m1[1] * m2[6] + m1[2] * m2[10] + m1[3] * m2[14];
			m[3] = m1[0] * m2[3] + m1[1] * m2[7] + m1[2] * m2[11] + m1[3] * m2[15];

			m[4] = m1[4] * m2[0] + m1[5] * m2[4] + m1[6] * m2[8] + m1[7] * m2[12];
			m[5] = m1[4] * m2[1] + m1[5] * m2[5] + m1[6] * m2[9] + m1[7] * m2[13];
			m[6] = m1[4] * m2[2] + m1[5] * m2[6] + m1[6] * m2[10] + m1[7] * m2[14];
			m[7] = m1[4] * m2[3] + m1[5] * m2[7] + m1[6] * m2[11] + m1[7] * m2[15];

			m[8] = m1[8] * m2[0] + m1[9] * m2[4] + m1[10] * m2[8] + m1[11] * m2[12];
			m[9] = m1[8] * m2[1] + m1[9] * m2[5] + m1[10] * m2[9] + m1[11] * m2[13];
			m[10] = m1[8] * m2[2] + m1[9] * m2[6] + m1[10] * m2[10] + m1[11] * m2[14];
			m[11] = m1[8] * m2[3] + m1[9] * m2[7] + m1[10] * m2[11] + m1[11] * m2[15];

			m[12] = m1[12] * m2[0] + m1[13] * m2[4] + m1[14] * m2[8] + m1[15] * m2[12];
			m[13] = m1[12] * m2[1] + m1[13] * m2[5] + m1[14] * m2[9] + m1[15] * m2[13];
			m[14] = m1[12] * m2[2] + m1[13] * m2[6] + m1[14] * m2[10] + m1[15] * m2[14];
			m[15] = m1[12] * m2[3] + m1[13] * m2[7] + m1[14] * m2[11] + m1[15] * m2[15];

			return *this;
		}

		//! 将矩阵初始化成单位矩阵,已验证
		inline CMatrix4& CMatrix4::MakeIdentity()
		{
			memset(m, 0, 16 * sizeof(float32));
			m[0] = m[5] = m[10] = m[15] = 1.0f;
			return *this;
		}

		/*
		check identity with epsilon
		solve floating range problems..
		*/
		//! 矩阵是否为单位矩阵,已验证
		inline bool CMatrix4::IsIdentity() const
		{
			if (!equals(m[12], 0.0f) || !equals(m[13], 0.0f) || !equals(m[14], 0.0f) || !equals(m[15], 1.0f))
				return false;

			if (!equals(m[0], 1.0f) || !equals(m[1], 0.0f) || !equals(m[2], 0.0f) || !equals(m[3], 0.0f))
				return false;

			if (!equals(m[4], 0.0f) || !equals(m[5], 1.0f) || !equals(m[6], 0.0f) || !equals(m[7], 0.0f))
				return false;

			if (!equals(m[8], 0.0f) || !equals(m[9], 0.0f) || !equals(m[10], 1.0f) || !equals(m[11], 0.0f))
				return false;

			return true;
		}

		/* Check orthogonality of matrix. */
		//! 矩阵是否为正交矩阵，未验证
		inline bool CMatrix4::IsOrthogonal() const
		{
			float32 dp = m[0] * m[4] + m[1] * m[5] + m[2] * m[6] + m[3] * m[7];
			if (!iszero(dp)) return false;

			dp = m[0] * m[8] + m[1] * m[9] + m[2] * m[10] + m[3] * m[11];
			if (!iszero(dp)) return false;

			dp = m[0] * m[12] + m[1] * m[13] + m[2] * m[14] + m[3] * m[15];
			if (!iszero(dp)) return false;

			dp = m[4] * m[8] + m[5] * m[9] + m[6] * m[10] + m[7] * m[11];
			if (!iszero(dp)) return false;

			dp = m[4] * m[12] + m[5] * m[13] + m[6] * m[14] + m[7] * m[15];
			if (!iszero(dp)) return false;

			dp = m[8] * m[12] + m[9] * m[13] + m[10] * m[14] + m[11] * m[15];
			return (iszero(dp));
		}


		//! 计算逆矩阵,已验证
		inline bool CMatrix4::MakeInverse()
		{
			CMatrix4 temp;

			if (GetInverse(temp))
			{
				*this = temp;
				return true;
			}

			return false;
		}

		//! 获得逆矩阵,已验证
		inline bool CMatrix4::GetInverse(CMatrix4& out) const
		{
			/// Calculates the inverse of this Matrix
			/// The inverse is calculated using Cramers rule.
			/// If no inverse exists then 'false' is returned.

			const CMatrix4& m = *this;

			float32 d = (m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)) * (m(2, 2) * m(3, 3) - m(2, 3) * m(3, 2)) -
				(m(0, 0) * m(1, 2) - m(0, 2) * m(1, 0)) * (m(2, 1) * m(3, 3) - m(2, 3) * m(3, 1)) +
				(m(0, 0) * m(1, 3) - m(0, 3) * m(1, 0)) * (m(2, 1) * m(3, 2) - m(2, 2) * m(3, 1)) +
				(m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)) * (m(2, 0) * m(3, 3) - m(2, 3) * m(3, 0)) -
				(m(0, 1) * m(1, 3) - m(0, 3) * m(1, 1)) * (m(2, 0) * m(3, 2) - m(2, 2) * m(3, 0)) +
				(m(0, 2) * m(1, 3) - m(0, 3) * m(1, 2)) * (m(2, 0) * m(3, 1) - m(2, 1) * m(3, 0));

			if (iszero(d, FLT_MIN))
				return false;

			d = 1.0f / d;

			out(0, 0) = d * (m(1, 1) * (m(2, 2) * m(3, 3) - m(2, 3) * m(3, 2)) +
				m(1, 2) * (m(2, 3) * m(3, 1) - m(2, 1) * m(3, 3)) +
				m(1, 3) * (m(2, 1) * m(3, 2) - m(2, 2) * m(3, 1)));
			out(0, 1) = d * (m(2, 1) * (m(0, 2) * m(3, 3) - m(0, 3) * m(3, 2)) +
				m(2, 2) * (m(0, 3) * m(3, 1) - m(0, 1) * m(3, 3)) +
				m(2, 3) * (m(0, 1) * m(3, 2) - m(0, 2) * m(3, 1)));
			out(0, 2) = d * (m(3, 1) * (m(0, 2) * m(1, 3) - m(0, 3) * m(1, 2)) +
				m(3, 2) * (m(0, 3) * m(1, 1) - m(0, 1) * m(1, 3)) +
				m(3, 3) * (m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)));
			out(0, 3) = d * (m(0, 1) * (m(1, 3) * m(2, 2) - m(1, 2) * m(2, 3)) +
				m(0, 2) * (m(1, 1) * m(2, 3) - m(1, 3) * m(2, 1)) +
				m(0, 3) * (m(1, 2) * m(2, 1) - m(1, 1) * m(2, 2)));
			out(1, 0) = d * (m(1, 2) * (m(2, 0) * m(3, 3) - m(2, 3) * m(3, 0)) +
				m(1, 3) * (m(2, 2) * m(3, 0) - m(2, 0) * m(3, 2)) +
				m(1, 0) * (m(2, 3) * m(3, 2) - m(2, 2) * m(3, 3)));
			out(1, 1) = d * (m(2, 2) * (m(0, 0) * m(3, 3) - m(0, 3) * m(3, 0)) +
				m(2, 3) * (m(0, 2) * m(3, 0) - m(0, 0) * m(3, 2)) +
				m(2, 0) * (m(0, 3) * m(3, 2) - m(0, 2) * m(3, 3)));
			out(1, 2) = d * (m(3, 2) * (m(0, 0) * m(1, 3) - m(0, 3) * m(1, 0)) +
				m(3, 3) * (m(0, 2) * m(1, 0) - m(0, 0) * m(1, 2)) +
				m(3, 0) * (m(0, 3) * m(1, 2) - m(0, 2) * m(1, 3)));
			out(1, 3) = d * (m(0, 2) * (m(1, 3) * m(2, 0) - m(1, 0) * m(2, 3)) +
				m(0, 3) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) +
				m(0, 0) * (m(1, 2) * m(2, 3) - m(1, 3) * m(2, 2)));
			out(2, 0) = d * (m(1, 3) * (m(2, 0) * m(3, 1) - m(2, 1) * m(3, 0)) +
				m(1, 0) * (m(2, 1) * m(3, 3) - m(2, 3) * m(3, 1)) +
				m(1, 1) * (m(2, 3) * m(3, 0) - m(2, 0) * m(3, 3)));
			out(2, 1) = d * (m(2, 3) * (m(0, 0) * m(3, 1) - m(0, 1) * m(3, 0)) +
				m(2, 0) * (m(0, 1) * m(3, 3) - m(0, 3) * m(3, 1)) +
				m(2, 1) * (m(0, 3) * m(3, 0) - m(0, 0) * m(3, 3)));
			out(2, 2) = d * (m(3, 3) * (m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)) +
				m(3, 0) * (m(0, 1) * m(1, 3) - m(0, 3) * m(1, 1)) +
				m(3, 1) * (m(0, 3) * m(1, 0) - m(0, 0) * m(1, 3)));
			out(2, 3) = d * (m(0, 3) * (m(1, 1) * m(2, 0) - m(1, 0) * m(2, 1)) +
				m(0, 0) * (m(1, 3) * m(2, 1) - m(1, 1) * m(2, 3)) +
				m(0, 1) * (m(1, 0) * m(2, 3) - m(1, 3) * m(2, 0)));
			out(3, 0) = d * (m(1, 0) * (m(2, 2) * m(3, 1) - m(2, 1) * m(3, 2)) +
				m(1, 1) * (m(2, 0) * m(3, 2) - m(2, 2) * m(3, 0)) +
				m(1, 2) * (m(2, 1) * m(3, 0) - m(2, 0) * m(3, 1)));
			out(3, 1) = d * (m(2, 0) * (m(0, 2) * m(3, 1) - m(0, 1) * m(3, 2)) +
				m(2, 1) * (m(0, 0) * m(3, 2) - m(0, 2) * m(3, 0)) +
				m(2, 2) * (m(0, 1) * m(3, 0) - m(0, 0) * m(3, 1)));
			out(3, 2) = d * (m(3, 0) * (m(0, 2) * m(1, 1) - m(0, 1) * m(1, 2)) +
				m(3, 1) * (m(0, 0) * m(1, 2) - m(0, 2) * m(1, 0)) +
				m(3, 2) * (m(0, 1) * m(1, 0) - m(0, 0) * m(1, 1)));
			out(3, 3) = d * (m(0, 0) * (m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1)) +
				m(0, 1) * (m(1, 2) * m(2, 0) - m(1, 0) * m(2, 2)) +
				m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0)));

			return true;
		}


		// returns transposed matrix
		//! 获得本矩阵的转置矩阵,已验证
		inline CMatrix4 CMatrix4::GetTransposed() const
		{
			CMatrix4 t;
			GetTransposed(t);
			return t;
		}


		// returns transposed matrix
		//! 获得本矩阵的转置矩阵,已验证
		inline void CMatrix4::GetTransposed(CMatrix4& dest) const
		{
			dest.m[0] = m[0];
			dest.m[1] = m[4];
			dest.m[2] = m[8];
			dest.m[3] = m[12];

			dest.m[4] = m[1];
			dest.m[5] = m[5];
			dest.m[6] = m[9];
			dest.m[7] = m[13];

			dest.m[8] = m[2];
			dest.m[9] = m[6];
			dest.m[10] = m[10];
			dest.m[11] = m[14];

			dest.m[12] = m[3];
			dest.m[13] = m[7];
			dest.m[14] = m[11];
			dest.m[15] = m[15];
		}

		//! 根据给定的点设置矩阵的平移值，已验证
		inline CMatrix4& CMatrix4::SetTranslation(const Vector3Df& translation)
		{
			m[12] = (float32)translation.x;
			m[13] = (float32)translation.y;
			m[14] = (float32)translation.z;

			return *this;
		}

		//! 获得矩阵的平移值，已验证
		inline Vector3Df CMatrix4::GetTranslation() const
		{
			return Vector3Df(m[12], m[13], m[14]);
		}

		//! 根据给定的3个欧拉角创建旋转矩阵，这个是弧度制，未验证
		inline CMatrix4& CMatrix4::SetRotationRadians(const Vector3Df& rotation)
		{
			const float64 cr = cos(rotation.x);
			const float64 sr = sin(rotation.x);
			const float64 cp = cos(rotation.y);
			const float64 sp = sin(rotation.y);
			const float64 cy = cos(rotation.z);
			const float64 sy = sin(rotation.z);

			m[0] = (float32)(cp*cy);
			m[1] = (float32)(cp*sy);
			m[2] = (float32)(-sp);

			const float64 srsp = sr*sp;
			const float64 crsp = cr*sp;

			m[4] = (float32)(srsp*cy - cr*sy);
			m[5] = (float32)(srsp*sy + cr*cy);
			m[6] = (float32)(sr*cp);

			m[8] = (float32)(crsp*cy + sr*sy);
			m[9] = (float32)(crsp*sy - sr*cy);
			m[10] = (float32)(cr*cp);

			return *this;
		}

		//! 根据给定的3个欧拉角创建旋转矩阵，这个是角度制，未验证
		inline CMatrix4& CMatrix4::SetRotationDegrees(const Vector3Df& rotation)
		{
			return SetRotationRadians(rotation * (float32)DEGTORAD);
		}

		//! Sets matrix to rotation matrix of inverse angles given as parameters
		//! 根据给定的3个欧拉角创建反方向的旋转矩阵，这个是弧度制，未验证
		inline CMatrix4& CMatrix4::SetInverseRotationRadians(const Vector3Df& rotation)
		{
			float64 cx = cos(rotation.x);
			float64 sx = sin(rotation.x);
			float64 cy = cos(rotation.y);
			float64 sy = sin(rotation.y);
			float64 cz = cos(rotation.z);
			float64 sz = sin(rotation.z);

			m[0] = (float32)(cy*cz);
			m[4] = (float32)(cy*sz);
			m[8] = (float32)(-sy);

			float64 sxsy = sx*sy;
			float64 cxsy = cx*sy;

			m[1] = (float32)(sxsy*cz - cx*sz);
			m[5] = (float32)(sxsy*sz + cx*cz);
			m[9] = (float32)(sx*cy);

			m[2] = (float32)(cxsy*cz + sx*sz);
			m[6] = (float32)(cxsy*sz - sx*cz);
			m[10] = (float32)(cx*cy);

			return *this;
		}

		//! 根据给定的3个欧拉角创建反方向的旋转矩阵，这个是角度制，未验证
		inline CMatrix4& CMatrix4::SetInverseRotationDegrees(const Vector3Df& rotation)
		{
			return SetInverseRotationRadians(rotation * (float32)DEGTORAD);
		}

		//! Sets matrix to rotation matrix defined by axis and angle, assuming LH rotation
		//! 以给定的向量为旋转轴，创建旋转angle度的旋转矩阵，angle为弧度制，已验证
		inline CMatrix4& CMatrix4::SetRotationAxisRadians(float32 angle, const Vector3Df& axis)
		{
			const float64 c = cos(angle);
			const float64 s = sin(angle);
			const float64 t = 1.0 - c;

			const float64 tx = t * axis.x;
			const float64 ty = t * axis.y;
			const float64 tz = t * axis.z;

			const float64 sx = s * axis.x;
			const float64 sy = s * axis.y;
			const float64 sz = s * axis.z;

			m[0] = (float32)(tx * axis.x + c);
			m[1] = (float32)(tx * axis.y + sz);
			m[2] = (float32)(tx * axis.z - sy);

			m[4] = (float32)(ty * axis.x - sz);
			m[5] = (float32)(ty * axis.y + c);
			m[6] = (float32)(ty * axis.z + sx);

			m[8] = (float32)(tz * axis.x + sy);
			m[9] = (float32)(tz * axis.y - sx);
			m[10] = (float32)(tz * axis.z + c);

			return *this;
		}

		//! 根据给定的向量创建缩放矩阵，已验证
		inline CMatrix4& CMatrix4::SetScale(const Vector3Df& scale)
		{
			m[0] = scale.x;
			m[5] = scale.y;
			m[10] = scale.z;

			return *this;
		}

		inline Vector3Df CMatrix4::GetScale() const
		{
			// Deal with the 0 rotation case first
			// Prior to Irrlicht 1.6, we always returned this value.
			if (iszero(m[1]) && iszero(m[2]) && iszero(m[4]) && iszero(m[6]) && iszero(m[8]) && iszero(m[9]))
				return Vector3Df(m[0], m[5], m[10]);

			// We have to do the full calculation.
			return Vector3Df(sqrtf(m[0] * m[0] + m[1] * m[1] + m[2] * m[2]), sqrtf(m[4] * m[4] + m[5] * m[5] + m[6] * m[6]), sqrtf(m[8] * m[8] + m[9] * m[9] + m[10] * m[10]));
		}

		inline Vector4D CMatrix4::Transform(const Vector2D& vect) const
		{
			Vector4D out;
			out.x = vect.x*m[0] + vect.y*m[4] + m[12];
			out.y = vect.x*m[1] + vect.y*m[5] + m[13];
			out.z = vect.x*m[2] + vect.y*m[6] + m[14];
			out.w = vect.x*m[3] + vect.y*m[7] + m[15];
			return out;
		}

		inline Vector4D CMatrix4::Transform(const Vector3Df& vect) const
		{
			Vector4D out;
			out.x = vect.x*m[0] + vect.y*m[4] + vect.z*m[8] + m[12];
			out.y = vect.x*m[1] + vect.y*m[5] + vect.z*m[9] + m[13];
			out.z = vect.x*m[2] + vect.y*m[6] + vect.z*m[10] + m[14];
			out.w = vect.x*m[3] + vect.y*m[7] + vect.z*m[11] + m[15];
			return out;
		}

		inline Vector4D CMatrix4::Transform(const Vector4D& vect) const
		{
			Vector4D out;
			out.x = vect.x*m[0] + vect.y*m[4] + vect.z*m[8] + vect.w*m[12];
			out.y = vect.x*m[1] + vect.y*m[5] + vect.z*m[9] + vect.w*m[13];
			out.z = vect.x*m[2] + vect.y*m[6] + vect.z*m[10] + vect.w*m[14];
			out.w = vect.x*m[3] + vect.y*m[7] + vect.z*m[11] + vect.w*m[15];
			return out;
		}

		inline Vector2D CMatrix4::TransformCoord(const Vector2D& vect) const
		{
			Vector2D out;
			float32 w = vect.x*m[3] + vect.y*m[7] + m[15];
			out.x = (vect.x*m[0] + vect.y*m[4] + m[12]) / w;
			out.y = (vect.x*m[1] + vect.y*m[5] + m[13]) / w;
			return out;
		}

		inline Vector3Df CMatrix4::TransformCoord(const Vector3Df& vect) const
		{
			Vector3Df out;
			float32 w = vect.x*m[3] + vect.y*m[7] + vect.z*m[11] + m[15];
			out.x = (vect.x*m[0] + vect.y*m[4] + vect.z*m[8] + m[12]) / w;
			out.y = (vect.x*m[1] + vect.y*m[5] + vect.z*m[9] + m[13]) / w;
			out.z = (vect.x*m[2] + vect.y*m[6] + vect.z*m[10] + m[14]) / w;
			return out;
		}

		inline Vector2D CMatrix4::TransformNormal(const Vector2D& vect) const
		{
			Vector2D out;
			out.x = vect.x*m[0] + vect.y*m[4];
			out.y = vect.x*m[1] + vect.y*m[5];
			return out;
		}

		inline Vector3Df CMatrix4::TransformNormal(const Vector3Df& vect) const
		{
			Vector3Df out;
			out.x = vect.x*m[0] + vect.y*m[4] + vect.z*m[8];
			out.y = vect.x*m[1] + vect.y*m[5] + vect.z*m[9];
			out.z = vect.x*m[2] + vect.y*m[6] + vect.z*m[10];
			return out;
		}

		// Builds a right-handed perspective projection matrix based on a field of view
		//! 使用FOV建立一个右手坐标系的透视投影矩阵,已验证
		inline CMatrix4& CMatrix4::BuildProjectionMatrixPerspectiveFovRH(float32 fieldOfViewRadians, float32 aspectRatio, float32 zNear, float32 zFar)
		{
			const float64 h = 1.0 / tan(fieldOfViewRadians*0.5);
			//DEBUG_BREAK_IF(aspectRatio==0.f); //divide by zero

			const float64 w = h / aspectRatio;
			//DEBUG_BREAK_IF(zNear==zFar); //divide by zero

			m[0] = (float32)w;
			m[1] = 0;
			m[2] = 0;
			m[3] = 0;

			m[4] = 0;
			m[5] = (float32)h;
			m[6] = 0;
			m[7] = 0;

			m[8] = 0;
			m[9] = 0;
			m[10] = (float32)(zFar / (zNear - zFar)); // DirectX version
			//		m[10] = (float32)(zFar+zNear/(zNear-zFar)); // OpenGL version
			m[11] = -1;

			m[12] = 0;
			m[13] = 0;
			m[14] = (float32)(zNear*zFar / (zNear - zFar)); // DirectX version
			//		m[14] = (float32)(2.0f*zNear*zFar/(zNear-zFar)); // OpenGL version
			m[15] = 0;

			return *this;
		}


		// Builds a left-handed perspective projection matrix based on a field of view
		//! 使用FOV建立一个左手坐标系的透视投影矩阵,已验证
		inline CMatrix4& CMatrix4::BuildProjectionMatrixPerspectiveFovLH(float32 fieldOfViewRadians, float32 aspectRatio, float32 zNear, float32 zFar)
		{
			const float64 h = 1.0 / tan(fieldOfViewRadians*0.5);
			//DEBUG_BREAK_IF(aspectRatio==0.f); //divide by zero

			const float64 w = h / aspectRatio;
			//DEBUG_BREAK_IF(zNear==zFar); //divide by zero

			m[0] = (float32)w;
			m[1] = 0;
			m[2] = 0;
			m[3] = 0;

			m[4] = 0;
			m[5] = (float32)h;
			m[6] = 0;
			m[7] = 0;

			m[8] = 0;
			m[9] = 0;
			m[10] = (float32)(zFar / (zFar - zNear));
			m[11] = 1;

			m[12] = 0;
			m[13] = 0;
			m[14] = (float32)(-zNear*zFar / (zFar - zNear));
			m[15] = 0;

			return *this;
		}


		// Builds a left-handed perspective projection matrix based on a field of view, with far plane culling at infinity
		//! 使用FOV建立一个Z无限远的左手坐标系的透视投影矩阵，未验证
		inline CMatrix4& CMatrix4::BuildProjectionMatrixPerspectiveFovInfinityLH(float32 fieldOfViewRadians, float32 aspectRatio, float32 zNear, float32 epsilon)
		{
			const float64 h = 1.0 / tan(fieldOfViewRadians*0.5);
			//DEBUG_BREAK_IF(aspectRatio==0.f); //divide by zero

			const float64 w = h / aspectRatio;

			m[0] = (float32)w;
			m[1] = 0;
			m[2] = 0;
			m[3] = 0;

			m[4] = 0;
			m[5] = (float32)h;
			m[6] = 0;
			m[7] = 0;

			m[8] = 0;
			m[9] = 0;
			m[10] = (float32)(1.0f - epsilon);
			m[11] = 1;

			m[12] = 0;
			m[13] = 0;
			m[14] = (float32)(zNear*(epsilon - 1.f));
			m[15] = 0;

			return *this;
		}



		// Builds a right-handed perspective projection matrix.
		//! 使用宽高建立一个右手坐标系的透视投影矩阵,已验证
		inline CMatrix4& CMatrix4::BuildProjectionMatrixPerspectiveRH(float32 widthOfViewVolume, float32 heightOfViewVolume, float32 zNear, float32 zFar)
		{
			//DEBUG_BREAK_IF(widthOfViewVolume==0.f); //divide by zero
			//DEBUG_BREAK_IF(heightOfViewVolume==0.f); //divide by zero
			//DEBUG_BREAK_IF(zNear==zFar); //divide by zero

			m[0] = (float32)(2 * zNear / widthOfViewVolume);
			m[1] = 0;
			m[2] = 0;
			m[3] = 0;

			m[4] = 0;
			m[5] = (float32)(2 * zNear / heightOfViewVolume);
			m[6] = 0;
			m[7] = 0;

			m[8] = 0;
			m[9] = 0;
			m[10] = (float32)(zFar / (zNear - zFar));
			m[11] = -1;

			m[12] = 0;
			m[13] = 0;
			m[14] = (float32)(zNear*zFar / (zNear - zFar));
			m[15] = 0;

			return *this;
		}

		// Builds a left-handed perspective projection matrix.
		//! 使用宽高建立一个左手坐标系的透视投影矩阵,已验证
		inline CMatrix4& CMatrix4::BuildProjectionMatrixPerspectiveLH(float32 widthOfViewVolume, float32 heightOfViewVolume, float32 zNear, float32 zFar)
		{
			//DEBUG_BREAK_IF(widthOfViewVolume==0.f); //divide by zero
			//DEBUG_BREAK_IF(heightOfViewVolume==0.f); //divide by zero
			//DEBUG_BREAK_IF(zNear==zFar); //divide by zero

			m[0] = (float32)(2 * zNear / widthOfViewVolume);
			m[1] = 0;
			m[2] = 0;
			m[3] = 0;

			m[4] = 0;
			m[5] = (float32)(2 * zNear / heightOfViewVolume);
			m[6] = 0;
			m[7] = 0;

			m[8] = 0;
			m[9] = 0;
			m[10] = (float32)(zFar / (zFar - zNear));
			m[11] = 1;

			m[12] = 0;
			m[13] = 0;
			m[14] = (float32)(zNear*zFar / (zNear - zFar));
			m[15] = 0;

			return *this;
		}


		// Builds a left-handed orthogonal projection matrix.
		//! 使用宽高建立一个左手坐标系的正交投影矩阵,已验证
		inline CMatrix4& CMatrix4::BuildProjectionMatrixOrthoLH(float32 widthOfViewVolume, float32 heightOfViewVolume, float32 zNear, float32 zFar)
		{
			//DEBUG_BREAK_IF(widthOfViewVolume==0.f); //divide by zero
			//DEBUG_BREAK_IF(heightOfViewVolume==0.f); //divide by zero
			//DEBUG_BREAK_IF(zNear==zFar); //divide by zero

			m[0] = (float32)(2 / widthOfViewVolume);
			m[1] = 0;
			m[2] = 0;
			m[3] = 0;

			m[4] = 0;
			m[5] = (float32)(2 / heightOfViewVolume);
			m[6] = 0;
			m[7] = 0;

			m[8] = 0;
			m[9] = 0;
			m[10] = (float32)(1 / (zFar - zNear));
			m[11] = 0;

			m[12] = 0;
			m[13] = 0;
			m[14] = (float32)(zNear / (zNear - zFar));
			m[15] = 1;

			return *this;
		}


		// Builds a right-handed orthogonal projection matrix.
		//! 使用宽高建立一个右手坐标系的正交投影矩阵,已验证
		inline CMatrix4& CMatrix4::BuildProjectionMatrixOrthoRH(float32 widthOfViewVolume, float32 heightOfViewVolume, float32 zNear, float32 zFar)
		{
			//DEBUG_BREAK_IF(widthOfViewVolume==0.f); //divide by zero
			//DEBUG_BREAK_IF(heightOfViewVolume==0.f); //divide by zero
			//DEBUG_BREAK_IF(zNear==zFar); //divide by zero

			m[0] = (float32)(2 / widthOfViewVolume);
			m[1] = 0;
			m[2] = 0;
			m[3] = 0;

			m[4] = 0;
			m[5] = (float32)(2 / heightOfViewVolume);
			m[6] = 0;
			m[7] = 0;

			m[8] = 0;
			m[9] = 0;
			m[10] = (float32)(1 / (zNear - zFar));
			m[11] = 0;

			m[12] = 0;
			m[13] = 0;
			m[14] = (float32)(zNear / (zNear - zFar));
			m[15] = 1;

			return *this;
		}

		// Builds a left-handed look-at matrix.
		//! 建立一个左手坐标系的观察矩阵,已验证
		inline CMatrix4& CMatrix4::BuildCameraLookAtMatrixLH(const Vector3Df& position, const Vector3Df& target, const Vector3Df& upVector)
		{
			Vector3Df zaxis = target - position;
			zaxis.Normalize();

			Vector3Df xaxis = upVector.CrossProduct(zaxis);
			xaxis.Normalize();

			Vector3Df yaxis = zaxis.CrossProduct(xaxis);

			m[0] = xaxis.x;
			m[1] = yaxis.x;
			m[2] = zaxis.x;
			m[3] = 0;

			m[4] = xaxis.y;
			m[5] = yaxis.y;
			m[6] = zaxis.y;
			m[7] = 0;

			m[8] = xaxis.z;
			m[9] = yaxis.z;
			m[10] = zaxis.z;
			m[11] = 0;

			m[12] = -xaxis.DotProduct(position);
			m[13] = -yaxis.DotProduct(position);
			m[14] = -zaxis.DotProduct(position);
			m[15] = 1;

			return *this;
		}


		// Builds a right-handed look-at matrix.
		//! 建立一个右手坐标系的观察矩阵,已验证
		inline CMatrix4& CMatrix4::BuildCameraLookAtMatrixRH(const Vector3Df& position, const Vector3Df& target, const Vector3Df& upVector)
		{
			Vector3Df zaxis = position - target;
			zaxis.Normalize();

			Vector3Df xaxis = upVector.CrossProduct(zaxis);
			xaxis.Normalize();

			Vector3Df yaxis = zaxis.CrossProduct(xaxis);

			m[0] = xaxis.x;
			m[1] = yaxis.x;
			m[2] = zaxis.x;
			m[3] = 0;

			m[4] = xaxis.y;
			m[5] = yaxis.y;
			m[6] = zaxis.y;
			m[7] = 0;

			m[8] = xaxis.z;
			m[9] = yaxis.z;
			m[10] = zaxis.z;
			m[11] = 0;

			m[12] = -xaxis.DotProduct(position);
			m[13] = -yaxis.DotProduct(position);
			m[14] = -zaxis.DotProduct(position);
			m[15] = 1;

			return *this;
		}

		// Builds a matrix that flattens geometry into a plane.
		//! 建立一个阴影矩阵，point为0时光源为点光源，point为1时光源为方向光,未验证
		inline CMatrix4& CMatrix4::BuildShadowMatrix(const Vector3Df& light, Plane3Df plane, float32 point)
		{
			plane.normal.Normalize();
			const float32 d = plane.normal.DotProduct(light);

			m[0] = (float32)(-plane.normal.x * light.x + d);
			m[1] = (float32)(-plane.normal.x * light.y);
			m[2] = (float32)(-plane.normal.x * light.z);
			m[3] = (float32)(-plane.normal.x * point);

			m[4] = (float32)(-plane.normal.y * light.x);
			m[5] = (float32)(-plane.normal.y * light.y + d);
			m[6] = (float32)(-plane.normal.y * light.z);
			m[7] = (float32)(-plane.normal.y * point);

			m[8] = (float32)(-plane.normal.z * light.x);
			m[9] = (float32)(-plane.normal.z * light.y);
			m[10] = (float32)(-plane.normal.z * light.z + d);
			m[11] = (float32)(-plane.normal.z * point);

			m[12] = (float32)(-plane.d * light.x);
			m[13] = (float32)(-plane.d * light.y);
			m[14] = (float32)(-plane.d * light.z);
			m[15] = (float32)(-plane.d * point + d);

			return *this;
		}

		//! Builds a matrix that rotates from one vector to another
		/** \param from: vector to rotate from
		\param to: vector to rotate to

		http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToMatrix/index.htm
		*/
		//! 建立一个from向量到to向量的旋转矩阵，已验证
		inline CMatrix4& CMatrix4::BuildRotateFromTo(const Vector3Df& from, const Vector3Df& to)
		{
			// unit vectors
			Vector3Df f(from);
			Vector3Df t(to);
			f.Normalize();
			t.Normalize();

			// axis multiplication by sin
			Vector3Df vs(t.CrossProduct(f));

			// axis of rotation
			Vector3Df v(vs);
			v.Normalize();

			// cosinus angle
			float32 ca = f.DotProduct(t);

			Vector3Df vt(v * (1 - ca));

			m[0] = vt.x * v.x + ca;
			m[5] = vt.y * v.y + ca;
			m[10] = vt.z * v.z + ca;

			vt.x *= v.y;
			vt.z *= v.x;
			vt.y *= v.z;

			m[1] = vt.x - vs.z;
			m[2] = vt.z + vs.y;
			m[3] = 0;

			m[4] = vt.x + vs.z;
			m[6] = vt.y - vs.x;
			m[7] = 0;

			m[8] = vt.z - vs.y;
			m[9] = vt.y + vs.x;
			m[11] = 0;

			m[12] = 0;
			m[13] = 0;
			m[14] = 0;
			m[15] = 1;

			return *this;
		}
		
		// creates a new matrix as interpolated matrix from this and the passed one.
		//! 和另一个矩阵插值，time必须为0至1之间，已验证
		inline CMatrix4 CMatrix4::Interpolate(const CMatrix4& b, float32 time) const
		{
			CMatrix4 mat;

			for (uint32 i = 0; i < 16; i += 4)
			{
				mat.m[i + 0] = (float32)(m[i + 0] + (b.m[i + 0] - m[i + 0]) * time);
				mat.m[i + 1] = (float32)(m[i + 1] + (b.m[i + 1] - m[i + 1]) * time);
				mat.m[i + 2] = (float32)(m[i + 2] + (b.m[i + 2] - m[i + 2]) * time);
				mat.m[i + 3] = (float32)(m[i + 3] + (b.m[i + 3] - m[i + 3]) * time);
			}

			return mat;
		}

		//! Compare two matrices using the equal method
		inline bool CMatrix4::Equals(const CMatrix4& other, const float32 tolerance) const
		{
			for (int32 i = 0; i < 16; i++)
			{
				if (!equals(m[i], other.m[i], tolerance))
					return false;
			}

			return true;
		}
	}

	typedef Math::CMatrix4 Matrix4;
}
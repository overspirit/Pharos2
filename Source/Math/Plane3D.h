#pragma once

namespace Pharos
{
	namespace Math
	{
		//! Enumeration for intersection relations of 3d objects
		enum EIntersectionRelation3D
		{
			ISREL3D_FRONT = 0,
			ISREL3D_BACK,
			ISREL3D_PLANAR,
			ISREL3D_SPANNING,
			ISREL3D_CLIPPED
		};

		// 3D平面
		class Plane3D
		{
		public:
			// 构造函数
			Plane3D()
			{
				normal.Set(0, 1, 0);
				RecalculateD(Vector3Df(0, 0, 0));
			}

			Plane3D(const Vector3Df& point, const Vector3Df& normal)
			{
				this->normal = normal;
				RecalculateD(point);
			}

			Plane3D(float32 px, float32 py, float32 pz, float32 nx, float32 ny, float32 nz)
			{
				normal.Set(nx, ny, nz);
				RecalculateD(Vector3Df(px, py, pz));
			}

			Plane3D(const Vector3Df& point1, const Vector3Df& point2, const Vector3Df& point3)
			{
				SetPlane(point1, point2, point3);
			}

			Plane3D(const Vector3Df& normal, const float32 d)
			{
				this->normal = normal;
				this->d = d;
			}

			Plane3D(const Plane3D& other)
			{
				this->normal = other.normal;
				this->d = (float32)other.d;
			}

			// 运算符重载

			inline bool operator==(const Plane3D& other) const
			{
				return (equals(d, other.d) && normal == other.normal);
			}

			inline bool operator!=(const Plane3D& other) const
			{
				return (!equals(d, other.d) || normal != other.normal);
			}

			// functions

			//用点和向量初始化值
			void SetPlane(const Vector3Df& point, const Vector3Df& normal)
			{
				this->normal = normal;
				RecalculateD(point);
			}

			//用向量和距离初始化值
			void SetPlane(const Vector3Df& normal, float32 d)
			{
				this->normal = normal;
				this->d = d;
			}

			//用3点初始化平面
			void SetPlane(const Vector3Df& point1, const Vector3Df& point2, const Vector3Df& point3)
			{
				// creates the plane from 3 memberpoints
				normal = (point2 - point1).CrossProduct(point3 - point1);
				normal.Normalize();

				RecalculateD(point1);
			}


			//! 检测一个直线和平面是否有交点，如果有交点，outIntersection为交点的值
			// start 为直线上一点
			// lineVect 为直线的方向
			// outIntersection 为交点
			bool GetIntersectionWithLine(const Vector3Df& linePoint, const Vector3Df& lineVect, Vector3Df& outIntersection) const
			{
				float32 t2 = normal.DotProduct(lineVect);

				if (t2 == 0) return false;

				float32 t = -(normal.DotProduct(linePoint) + d) / t2;
				outIntersection = linePoint + (lineVect * t);
				return true;
			}

			//! 检测给定的线段和平面是否有交点，返回交点距起始点的长度百分比，比如返回0.5就是交点在两点的中间
			float32 GetKnownIntersectionWithLine(const Vector3Df& linePoint1, const Vector3Df& linePoint2) const
			{
				Vector3Df vect = linePoint2 - linePoint1;
				float32 t2 = (float32)normal.DotProduct(vect);
				return (float32)-((normal.DotProduct(linePoint1) + d) / t2);
			}

			//! 检测给定的线段和平面是否有交点，也就是平面必须在给定的两点中间
			bool GetIntersectionWithLimitedLine(const Vector3Df& linePoint1, const Vector3Df& linePoint2, Vector3Df& outIntersection) const
			{
				return (GetIntersectionWithLine(linePoint1, linePoint2 - linePoint1, outIntersection) &&
					outIntersection.IsBetweenPoints(linePoint1, linePoint2));
			}

			//! 获得给定点和平面的关系
			EIntersectionRelation3D ClassifyPointRelation(const Vector3Df& point) const
			{
				const float32 dis = normal.DotProduct(point) + d;

				if (dis < -ROUNDING_ERROR_f32)
					return ISREL3D_BACK;

				if (dis > ROUNDING_ERROR_f32)
					return ISREL3D_FRONT;

				return ISREL3D_PLANAR;
			}

			//! 通过法线和给定的点，计算该平面和原点的距离
			void RecalculateD(const Vector3Df& point)
			{
				d = -point.DotProduct(normal);
			}

			//! 获得该平面上距原点最近的点
			Vector3Df GetMemberPoint() const
			{
				return normal * -d;
			}

			//! 快速检测和另一个平面是否相交
			bool ExistsIntersection(const Plane3D& other) const
			{
				Vector3Df cross = other.normal.CrossProduct(normal);
				return cross.GetLength() > ROUNDING_ERROR_f32;
			}

			//! 检测是否和另一个平面是否相交，
			// outLinePoint 为交线上的一点
			// outLineVect 为交线的方向
			bool GetIntersectionWithPlane(const Plane3D& other, Vector3Df& outLinePoint, Vector3Df& outLineVect) const
			{
				const float32 fn00 = normal.GetLength();
				const float32 fn01 = normal.DotProduct(other.normal);
				const float32 fn11 = other.normal.GetLength();
				const float64 det = fn00*fn11 - fn01*fn01;

				if (fabs(det) < ROUNDING_ERROR_f64)
					return false;

				const float64 invdet = 1.0 / det;
				const float64 fc0 = (fn11*-d + fn01*other.d) * invdet;
				const float64 fc1 = (fn00*-other.d + fn01*d) * invdet;

				outLineVect = normal.CrossProduct(other.normal);
				outLinePoint = normal*(float32)fc0 + other.normal*(float32)fc1;
				return true;
			}

			//! 检测和另外两个平面是否有交点，outPoint 为返回的交点
			bool GetIntersectionWithPlanes(const Plane3D& o1, const Plane3D& o2, Vector3Df& outPoint) const
			{
				Vector3Df linePoint, lineVect;
				if (GetIntersectionWithPlane(o1, linePoint, lineVect))
					return o2.GetIntersectionWithLine(linePoint, lineVect, outPoint);

				return false;
			}

			//检测球体和面是否相交
			bool GetIntersectionWithSphere(const Vector3Df& center, float32 raidus)
			{
				return ((float32)fabs((float64)GetDistanceTo(center)) <= raidus);
			}

			//! 检测给定的向量是否指向平面的正方向
			bool IsFrontFacing(const Vector3Df& lookDirection) const
			{
				return (normal.DotProduct(lookDirection) <= 0.0f);
			}

			//! 获得给定的点到平面的距离
			float32 GetDistanceTo(const Vector3Df& point) const
			{
				return point.DotProduct(normal) + d;
			}

			//! 归一化向量和长度
			void Normalize()
			{
				float32 mag = normal.GetLength();
				normal /= mag;
				d /= mag;
			}

		public:
			//! normal vector of the plane.
			Vector3Df normal;

			//! Distance from origin.
			float32 d;
		};
	}
	//! Typedef for a float32 3d plane.
	typedef Math::Plane3D Plane3Df;

	//! Typedef for an integer 3d plane.
	//typedef Math::Plane3D<int32> Plane3Di;
}
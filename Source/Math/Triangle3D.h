#pragma once

namespace Pharos
{
	namespace Math
	{
		// 3D空间内的三角形
		class Triangle3D
		{
		public:
			//! 默认构造函数
			Triangle3D()
			{

			}

			//! 使用给定的三个点初始化三角形
			Triangle3D(const Vector3Df& v1, const Vector3Df& v2, const Vector3Df& v3)
			{
				pointA = v1;
				pointB = v2;
				pointC = v3;
			}

			Triangle3D(const Triangle3D& other)
			{
				pointA = other.pointA;
				pointB = other.pointB;
				pointC = other.pointC;
			}

			//! 等于符号重载
			bool operator==(const Triangle3D& other) const
			{
				return other.pointA == pointA && other.pointB == pointB && other.pointC == pointC;
			}

			//! 不等于符号重载
			bool operator!=(const Triangle3D& other) const
			{
				return !(*this == other);
			}

			//! 返回一个距给定点最近的在三角形边上的点
			Vector3Df ClosestPointOnTriangle(const Vector3Df& p) const
			{
				const Vector3Df rab = Line3D(pointA, pointB).GetClosestPoint(p);
				const Vector3Df rbc = Line3D(pointB, pointC).GetClosestPoint(p);
				const Vector3Df rca = Line3D(pointC, pointA).GetClosestPoint(p);

				const float32 d1 = rab.GetDistanceFrom(p);
				const float32 d2 = rbc.GetDistanceFrom(p);
				const float32 d3 = rca.GetDistanceFrom(p);

				if (d1 < d2)
					return d1 < d3 ? rab : rca;

				return d2 < d3 ? rbc : rca;
			}

			//! 检测一个点是否在三角形三个点所形成的体内
			bool IsPointInside(const Vector3Df& p) const
			{
				Vector3Df af64(pointA.x, pointA.y, pointA.z);
				Vector3Df bf64(pointB.x, pointB.y, pointB.z);
				Vector3Df cf64(pointC.x, pointC.y, pointC.z);
				Vector3Df pf64(p.x, p.y, p.z);
				return (IsOnSameSide(pf64, af64, bf64, cf64) &&
					IsOnSameSide(pf64, bf64, af64, cf64) &&
					IsOnSameSide(pf64, cf64, af64, bf64));
			}

			//快速检测一个点是否在三角形三个点所形成的体内
			//! Check if a point is inside the triangle (border-points count also as inside)
			/** This method uses a barycentric coordinate system.
			It is faster than isPointInside but is more susceptible to floating point rounding
			errors. This will especially be noticable when the FPU is in single precision mode
			(which is for example set on default by Direct3D).
			\param p Point to test. Assumes that this point is already
			on the plane of the triangle.
			\return True if point is inside the triangle, otherwise false. */
			bool IsPointInsideFast(const Vector3Df& p) const
			{
				const Vector3Df a = pointC - pointA;
				const Vector3Df b = pointB - pointA;
				const Vector3Df c = p - pointA;

				const float64 dotAA = a.DotProduct(a);
				const float64 dotAB = a.DotProduct(b);
				const float64 dotAC = a.DotProduct(c);
				const float64 dotBB = b.DotProduct(b);
				const float64 dotBC = b.DotProduct(c);

				// get coordinates in barycentric coordinate system
				const float64 invDenom = 1 / (dotAA * dotBB - dotAB * dotAB);
				const float64 u = (dotBB * dotAC - dotAB * dotBC) * invDenom;
				const float64 v = (dotAA * dotBC - dotAB * dotAC) * invDenom;

				// We count border-points as inside to keep downward compatibility.
				// Rounding-error also needed for some test-cases.
				return (u > -ROUNDING_ERROR_f32) && (v >= 0) && (u + v < 1 + ROUNDING_ERROR_f32);

			}

			//! 检测给定的线段是否和三角形有交点，在线段外部无交点
			bool GetIntersectionWithLimitedLine(const Line3D& line, Vector3Df& outIntersection) const
			{
				return GetIntersectionWithLine(line.start, line.GetVector().Normalize(), outIntersection)
					&& outIntersection.IsBetweenPoints(line.start, line.end);
			}

			//! 检测一个直线是否和三角形有交点，交点必须三角形内
			bool GetIntersectionWithLine(const Vector3Df& linePoint, const Vector3Df& lineVect, Vector3Df& outIntersection) const
			{
				if (GetIntersectionOfPlaneWithLine(linePoint, lineVect, outIntersection))
					return IsPointInside(outIntersection);

				return false;
			}


			//! 检测一个直线是否和三角形有交点，交点可以落在三角形所在平面的任意位置
			bool GetIntersectionOfPlaneWithLine(const Vector3Df& linePoint, const Vector3Df& lineVect, Vector3Df& outIntersection) const
			{
				// Work with float64 to get more precise results (makes enough difference to be worth the casts).
				const Vector3Df linePointf64(linePoint.x, linePoint.y, linePoint.z);
				const Vector3Df lineVectf64(lineVect.x, lineVect.y, lineVect.z);
				Vector3Df outIntersectionf64;

				Triangle3D trianglef64(Vector3Df(pointA.x, pointA.y, pointA.z)
					, Vector3Df(pointB.x, pointB.y, pointB.z)
					, Vector3Df(pointC.x, pointC.y, pointC.z));
				const Vector3Df normalf64 = trianglef64.GetNormal();

				float32 t2;

				if (iszero(t2 = normalf64.DotProduct(lineVectf64)))
					return false;

				float32 d = trianglef64.pointA.DotProduct(normalf64);
				float32 t = -(normalf64.DotProduct(linePointf64) - d) / t2;
				outIntersectionf64 = linePointf64 + (lineVectf64 * t);

				outIntersection.x = (float32)outIntersectionf64.x;
				outIntersection.y = (float32)outIntersectionf64.y;
				outIntersection.z = (float32)outIntersectionf64.z;
				return true;
			}

			//! 获得三角形所在面的法线方向
			Vector3Df GetNormal() const
			{
				Vector3Df normal = (pointB - pointA).CrossProduct(pointC - pointA);
				return normal.Normalize();
			}

			//! 检测给定的向量是否指向三角形的正方向
			bool isFrontFacing(const Vector3Df& lookDirection) const
			{
				const Vector3Df n = GetNormal();
				return ((float32)n.DotProduct(lookDirection) <= 0.0f);
			}

			//! 获得三角形所在的平面
			Plane3D GetPlane() const
			{
				return Plane3D(pointA, pointB, pointC);
			}

			//! 获得三角形面积
			float32 GetArea() const
			{
				return (pointB - pointA).CrossProduct(pointC - pointA).GetLength() * 0.5f;
			}

			/**	计算一个三角形的中心点	*/
			Vector3Df GetCenter()
			{
				return (pointA + pointB + pointC) / (float32)3;
			}

			//! 设置新值
			void Set(const Vector3Df& a, const Vector3Df& b, const Vector3Df& c)
			{
				pointA = a;
				pointB = b;
				pointC = c;
			}

			//! the three points of the triangle
			Vector3Df pointA;
			Vector3Df pointB;
			Vector3Df pointC;

		private:
			// Using float64 instead of  to avoid integer overflows when float32=int (maybe also less floating point troubles).
			bool IsOnSameSide(const Vector3Df& p1, const Vector3Df& p2,
				const Vector3Df& a, const Vector3Df& b) const
			{
				Vector3Df bminusa = b - a;
				Vector3Df cp1 = bminusa.CrossProduct(p1 - a);
				Vector3Df cp2 = bminusa.CrossProduct(p2 - a);
				float64 res = cp1.DotProduct(cp2);
				if (res < 0)
				{
					// This catches some floating point troubles.
					// Unfortunately slightly expensive and we don't really know the best epsilon for iszero.
					Vector3Df cp1 = bminusa.Normalize().CrossProduct((p1 - a).Normalize());
					if (iszero(cp1.x, ROUNDING_ERROR_f32)
						&& iszero(cp1.y, ROUNDING_ERROR_f32)
						&& iszero(cp1.z, ROUNDING_ERROR_f32))
					{
						res = 0.f;
					}
				}
				return (res >= 0.0f);
			}
		};
	}

	typedef Math::Triangle3D Triangle3Df;
}
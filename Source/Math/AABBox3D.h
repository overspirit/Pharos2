#pragma once

namespace Pharos
{
	namespace Math
	{
		//! 3D包围盒
		class AABBox3D
		{
		public:
			//! 默认构造函数
			AABBox3D()
			{
				minEdge.Set(-1, -1, -1);
				maxEdge.Set(1, 1, 1);
			}

			//! 使用两点初始化值
			AABBox3D(const Vector3Df& min, const Vector3Df& max)
			{
				minEdge = min;
				maxEdge = max;
			}

			//! 使用相同的一个点初始化值
			AABBox3D(const Vector3Df& init)
			{
				minEdge = init;
				maxEdge = init;
			}

			//! 使用两点初始化值
			AABBox3D(float32 minx, float32 miny, float32 minz, float32 maxx, float32 maxy, float32 maxz)
			{
				minEdge.Set(minx, miny, minz);
				maxEdge.Set(maxx, maxy, maxz);
			}

			// 运算符重载

			//! 等于运算符重载
			inline bool operator==(const AABBox3D& other) const
			{
				return (minEdge == other.minEdge && other.maxEdge == maxEdge);
			}

			//! 不等于运算符重载
			inline bool operator!=(const AABBox3D& other) const
			{
				return !(minEdge == other.minEdge && other.maxEdge == maxEdge);
			}

			// functions

			//! 使用一个点初始化值
			void Reset(float32 x, float32 y, float32 z)
			{
				maxEdge.Set(x, y, z);
				minEdge.Set(x, y, z);
			}

			//! 使用另一个包围盒初始化值
			void Reset(const AABBox3D& other)
			{
				maxEdge = other.maxEdge;
				minEdge = other.minEdge;
			}

			//! 使用同一个点初始化值
			void Reset(const Vector3Df& initValue)
			{
				maxEdge = initValue;
				minEdge = initValue;
			}

			//! 添加一个点到包围盒中，如果该点在包围盒外部，包围盒会增长大小将该点包括在内
			void AddInternalPoint(const Vector3Df& p)
			{
				AddInternalPoint(p.x, p.y, p.z);
			}

			//! 添加一个包围盒，如果包围盒不完全在内部，会增加包围盒大小将该包围盒包括在内
			void AddInternalBox(const AABBox3D& b)
			{
				AddInternalPoint(b.maxEdge);
				AddInternalPoint(b.minEdge);
			}

			//! 添加一个点到包围盒中，如果该点在包围盒外部，包围盒会增长大小将该点包括在内
			void AddInternalPoint(float32 x, float32 y, float32 z)
			{
				if (x > maxEdge.x) maxEdge.x = x;
				if (y > maxEdge.y) maxEdge.y = y;
				if (z > maxEdge.z) maxEdge.z = z;

				if (x < minEdge.x) minEdge.x = x;
				if (y < minEdge.y) minEdge.y = y;
				if (z < minEdge.z) minEdge.z = z;
			}

			//! 获得中心点
			Vector3Df GetCenter() const
			{
				return (minEdge + maxEdge) / 2;
			}

			//! 获得外接包围球的半径
			float32 GetRadius() const
			{
				return minEdge.GetDistanceFrom(maxEdge) / (float32)2;
			}

			//! 获得外接包围球的半径的平方
			float32 GetRadiusSQ() const
			{
				return minEdge.GetDistanceFromSQ(maxEdge) / (float32)4;
			}

			//! 获得包围盒长度区间向量
			Vector3Df GetExtent() const
			{
				return maxEdge - minEdge;
			}

			//! 检测包围盒是否为空
			bool IsEmpty() const
			{
				return minEdge.Equals(maxEdge);
			}

			//! 获得包围盒体积
			float32 GetVolume() const
			{
				const Vector3Df e = GetExtent();
				return e.x * e.y * e.z;
			}

			//! 获得包围盒的表面积
			float32 GetArea() const
			{
				const Vector3Df e = GetExtent();
				return 2 * (e.x*e.y + e.x*e.z + e.y*e.z);
			}

			//! 获得包围盒上的8个点
			void GetEdges(Vector3Df* edges) const
			{
				const Vector3Df middle = GetCenter();
				const Vector3Df diag = middle - maxEdge;

				/*
				Edges are stored in this way:
				Hey, am I an ascii artist, or what? :) niko.
				  /3---------/7
				 / |        / |
				/  |       /  |
				1---------5   |
				|  /2- - -|- -6
				| /       |  /
				|/        | /
				0---------4/
				*/

				edges[0].Set(middle.x + diag.x, middle.y + diag.y, middle.z + diag.z);
				edges[1].Set(middle.x + diag.x, middle.y - diag.y, middle.z + diag.z);
				edges[2].Set(middle.x + diag.x, middle.y + diag.y, middle.z - diag.z);
				edges[3].Set(middle.x + diag.x, middle.y - diag.y, middle.z - diag.z);
				edges[4].Set(middle.x - diag.x, middle.y + diag.y, middle.z + diag.z);
				edges[5].Set(middle.x - diag.x, middle.y - diag.y, middle.z + diag.z);
				edges[6].Set(middle.x - diag.x, middle.y + diag.y, middle.z - diag.z);
				edges[7].Set(middle.x - diag.x, middle.y - diag.y, middle.z - diag.z);
			}

			//! 修复，保证最大点大于最小点
			void Repair()
			{
				float32 t;

				if (minEdge.x > maxEdge.x)
				{
					t = minEdge.x; minEdge.x = maxEdge.x; maxEdge.x = t;
				}

				if (minEdge.y > maxEdge.y)
				{
					t = minEdge.y; minEdge.y = maxEdge.y; maxEdge.y = t;
				}

				if (minEdge.z > maxEdge.z)
				{
					t = minEdge.z; minEdge.z = maxEdge.z; maxEdge.z = t;
				}
			}

			//! 和另一个包围盒插值，返回一个新的包围盒
			AABBox3D GetInterpolated(const AABBox3D& other, float32 d) const
			{
				float32 inv = 1.0f - d;
				return AABBox3D((other.minEdge*inv) + (minEdge*d), (other.maxEdge*inv) + (maxEdge*d));
			}

			//! 检测一个点是否在包围盒内部，在边上也视为在包围盒内
			bool IsPointInside(const Vector3Df& p) const
			{
				return (p.x >= minEdge.x
					&& p.x <= maxEdge.x
					&& p.y >= minEdge.y
					&& p.y <= maxEdge.y
					&& p.z >= minEdge.z
					&& p.z <= maxEdge.z);
			}

			//!//! 检测一个点是否完全在包围盒内部，即在边上视为不完全在包围盒内
			bool IsPointTotalInside(const Vector3Df& p) const
			{
				return (p.x > minEdge.x
					&& p.x < maxEdge.x
					&& p.y > minEdge.y
					&& p.y < maxEdge.y
					&& p.z > minEdge.z
					&& p.z < maxEdge.z);
			}

			//! 检测另一个包围盒是否完全在本包围盒内部
			bool IsFullInside(const AABBox3D& other) const
			{
				return (minEdge.x >= other.minEdge.x
					&& minEdge.y >= other.minEdge.y
					&& minEdge.z >= other.minEdge.z
					&& maxEdge.x <= other.maxEdge.x
					&& maxEdge.y <= other.maxEdge.y
					&& maxEdge.z <= other.maxEdge.z);
			}

			//检测球体和包围盒是否相交
			bool IntersectsWithSphere(const Vector3Df& center, float32 radius)
			{
				// just test facing planes, early fail if sphere is totally outside
				if (center.x < minEdge.x && minEdge.x - center.x > radius)
					return false;

				if (center.x > maxEdge.x && center.x - maxEdge.x > radius)
					return false;

				if (center.y < minEdge.y && minEdge.y - center.y > radius)
					return false;

				if (center.y > maxEdge.y && center.y - maxEdge.y > radius)
					return false;

				if (center.z < minEdge.z && minEdge.z - center.z > radius)
					return false;

				if (center.z > maxEdge.z && center.z - maxEdge.z > radius)
					return false;

				// Must intersect
				return true;
			}

			//检测面和包围盒是否相交
			bool IntersectsWithPlane(const Plane3D& plane)
			{
				Vector3Df pEdges[8];
				GetEdges(pEdges);

				EIntersectionRelation3D lastSide = plane.ClassifyPointRelation(pEdges[0]);
				for (int32 i = 1; i < 8; i++)
				{
					if (plane.ClassifyPointRelation(pEdges[i]) != lastSide)
					{
						return true;
					}
				}

				return false;
			}

			//! 检测两个包围盒是否相交
			bool IntersectsWithBox(const AABBox3D& other) const
			{
				return (minEdge.x <= other.maxEdge.x
					&& minEdge.y <= other.maxEdge.y
					&& minEdge.z <= other.maxEdge.z
					&& maxEdge.x >= other.minEdge.x
					&& maxEdge.y >= other.minEdge.y
					&& maxEdge.z >= other.minEdge.z);
			}

			//! 检测一条直线和包围盒是否有交点
			bool IntersectsWithLimitedLine(const Vector3Df& start, const Vector3Df& end) const
			{
				return IntersectsWithLimitedLine(Line3D(start, end));
			}

			//! 检测一条直线和包围盒是否有交点
			bool IntersectsWithLimitedLine(const Line3D& line) const
			{
				Vector3Df linemiddle = line.GetMiddle();
				Vector3Df linevect = line.GetVector().Normalize();
				float32 halflength = (float32)(line.GetLength() * 0.5);

				const Vector3Df e = GetExtent() * (float32)0.5;
				const Vector3Df t = GetCenter() - linemiddle;

				if ((fabs(t.x) > e.x + halflength * fabs(linevect.x)) ||
					(fabs(t.y) > e.y + halflength * fabs(linevect.y)) ||
					(fabs(t.z) > e.z + halflength * fabs(linevect.z)))
					return false;

				float32 r = e.y * (float32)fabs(linevect.z) + e.z * (float32)fabs(linevect.y);
				if (fabs(t.y*linevect.z - t.z*linevect.y) > r)
					return false;

				r = e.x * (float32)fabs(linevect.z) + e.z * (float32)fabs(linevect.x);
				if (fabs(t.z*linevect.x - t.x*linevect.z) > r)
					return false;

				r = e.x * (float32)fabs(linevect.y) + e.y * (float32)fabs(linevect.x);
				if (fabs(t.x*linevect.y - t.y*linevect.x) > r)
					return false;

				return true;
			}

			//! 检测一个三角形是否完全在包围盒内部
			bool IsTriangleTotalInside(const Triangle3D& tri) const
			{
				return (IsPointInside(tri.pointA) && IsPointInside(tri.pointB) && IsPointInside(tri.pointC));
			}

			//! 检测一个三角形是否完全在包围盒外部
			bool IsTriangleTotalOutside(const Triangle3D& tri) const
			{
				return ((tri.pointA.x > maxEdge.x && tri.pointB.x > maxEdge.x && tri.pointC.x > maxEdge.x) ||
					(tri.pointA.y > maxEdge.y && tri.pointB.y > maxEdge.y && tri.pointC.y > maxEdge.y) ||
					(tri.pointA.z > maxEdge.z && tri.pointB.z > maxEdge.z && tri.pointC.z > maxEdge.z) ||
					(tri.pointA.x < minEdge.x && tri.pointB.x < minEdge.x && tri.pointC.x < minEdge.x) ||
					(tri.pointA.y < minEdge.y && tri.pointB.y < minEdge.y && tri.pointC.y < minEdge.y) ||
					(tri.pointA.z < minEdge.z && tri.pointB.z < minEdge.z && tri.pointC.z < minEdge.z));
			}

			//! 检测一个三角形是否和包围盒发生碰撞
			bool IntersectsWithTriangle(const Triangle3D& tri)
			{
				if (IsTriangleTotalInside(tri)) return true;

				if (!IsTriangleTotalOutside(tri)) return true;

				return false;
			}

			//! 获得给定的平面和包围盒的关系
			EIntersectionRelation3D ClassifyPlaneRelation(const Plane3D& plane) const
			{
				Vector3Df nearPoint(maxEdge);
				Vector3Df farPoint(minEdge);

				if (plane.normal.x > (float32)0)
				{
					nearPoint.x = minEdge.x;
					farPoint.x = maxEdge.x;
				}

				if (plane.normal.y > (float32)0)
				{
					nearPoint.y = minEdge.y;
					farPoint.y = maxEdge.y;
				}

				if (plane.normal.z > (float32)0)
				{
					nearPoint.z = minEdge.z;
					farPoint.z = maxEdge.z;
				}

				if (plane.normal.DotProduct(nearPoint) + plane.d > (float32)0)
					return ISREL3D_FRONT;

				if (plane.normal.DotProduct(farPoint) + plane.d > (float32)0)
					return ISREL3D_CLIPPED;

				return ISREL3D_BACK;
			}

			//! The near edge
			Vector3Df minEdge;

			//! The far edge
			Vector3Df maxEdge;
		};
	}

	//! Typedef for a float32 3d bounding box.
	typedef Math::AABBox3D AABBox3Df;

	//! Typedef for an integer 3d bounding box.
	//typedef Math::AABBox3D<int32> AABBox3Di;
}
#pragma once

namespace Pharos
{
	namespace Math
	{
		//平截头体
		class CFrustum
		{
		public:
			CFrustum(void)
			{

			}	

		public:
			//检测一个给定的Bound Box是否部分在平截头体里，
			bool BoxInFrustum(const AABBox3Df& box) const
			{
				Vector3Df pt[8];
				box.GetEdges(pt);

				// Go through all of the corners of the box and check then again each plane
				// in the frustum.  If all of them are behind one of the planes, then it most
				// like is not in the frustum.
				for (int32 i = 0; i < 6; i++)
				{
					if (m_Planes[i].ClassifyPointRelation(pt[0]) == ISREL3D_FRONT) continue;
					if (m_Planes[i].ClassifyPointRelation(pt[1]) == ISREL3D_FRONT) continue;
					if (m_Planes[i].ClassifyPointRelation(pt[2]) == ISREL3D_FRONT) continue;
					if (m_Planes[i].ClassifyPointRelation(pt[3]) == ISREL3D_FRONT) continue;
					if (m_Planes[i].ClassifyPointRelation(pt[4]) == ISREL3D_FRONT) continue;
					if (m_Planes[i].ClassifyPointRelation(pt[5]) == ISREL3D_FRONT) continue;
					if (m_Planes[i].ClassifyPointRelation(pt[6]) == ISREL3D_FRONT) continue;
					if (m_Planes[i].ClassifyPointRelation(pt[7]) == ISREL3D_FRONT) continue;

					// If we get here, it isn't in the frustum
					return false;
				}

				// Return a true for the box being inside of the frustum
				return true;
			}

			//检测一个Bound Box是否完全在平截头体里
			bool BoxAllInFrustum(const AABBox3Df& box) const
			{
				Vector3Df pt[8];
				box.GetEdges(pt);

				for (int32 i = 0; i < 6; i++)
				{
					if (m_Planes[i].ClassifyPointRelation(pt[0]) != ISREL3D_FRONT) return false;
					if (m_Planes[i].ClassifyPointRelation(pt[1]) != ISREL3D_FRONT) return false;
					if (m_Planes[i].ClassifyPointRelation(pt[2]) != ISREL3D_FRONT) return false;
					if (m_Planes[i].ClassifyPointRelation(pt[3]) != ISREL3D_FRONT) return false;
					if (m_Planes[i].ClassifyPointRelation(pt[4]) != ISREL3D_FRONT) return false;
					if (m_Planes[i].ClassifyPointRelation(pt[5]) != ISREL3D_FRONT) return false;
					if (m_Planes[i].ClassifyPointRelation(pt[6]) != ISREL3D_FRONT) return false;
					if (m_Planes[i].ClassifyPointRelation(pt[7]) != ISREL3D_FRONT) return false;
				}

				return true;
			}

			//检测一个球体是否在平截头体里
			bool SphereInFrustum(const Vector3Df& vtCenter, float32 fRadius) const
			{
				// For each plane, see if sphere is on negative side
				// If so, object is not visible
				for (int32 i = 0; i < 6; i++)
				{
					// If the distance from sphere center to plane is negative, and 'more negative' 
					// than the radius of the sphere, sphere is outside frustum
					if (m_Planes[i].GetDistanceTo(vtCenter) < -fRadius)
					{
						// ALL corners on negative side therefore out of view
						return false;
					}

				}

				return true;
			}

		public:
			Plane3Df m_Planes[6];
		};
	}

	typedef Math::CFrustum Frustum;
}
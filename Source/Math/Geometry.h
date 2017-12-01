#pragma once

namespace Pharos
{
	namespace Math
	{
		//根据纹理坐标计算切线和副法线
		REALINLINE void CalculateTangentSpace(
			const Vector3Df& pos1, const Vector2Df& tex1,
			const Vector3Df& pos2, const Vector2Df& tex2,
			const Vector3Df& pos3, const Vector2Df& tex3,
			Vector3Df& tangent, Vector3Df& binormal)
		{
			// calculate tangent vectors ---------------------------

			Vector3Df vA = pos2 - pos1;
			Vector3Df vB = pos3 - pos1;

			float32 fDeltaU1 = tex2.x - tex1.x;
			float32 fDeltaU2 = tex3.x - tex1.x;
			float32 fDeltaV1 = tex2.y - tex1.y;
			float32 fDeltaV2 = tex3.y - tex1.y;

			float32 div = (fDeltaU1 * fDeltaV2 - fDeltaU2 * fDeltaV1);

			if (isnotzero(div))
			{
				//	2D triangle area = (u1*v2-u2*v1)/2
				// weight the tangent vectors by the UV triangles area size (fix problems with base UV assignment)
				float32 fAreaMul2 = fabsf(div);

				float32 a = fDeltaV2 / div;
				float32 b = -fDeltaV1 / div;
				float32 c = -fDeltaU2 / div;
				float32 d = fDeltaU1 / div;

				tangent = ((vA * a + vB * b) * fAreaMul2).Normalize();
				binormal = ((vA * c + vB * d) * fAreaMul2).Normalize();
			}
		}
	}
}
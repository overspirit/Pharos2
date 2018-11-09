#pragma once

namespace Pharos
{
	namespace Render
	{
		struct DecalVertex
		{
			Vector3Df	pos;
			Vector2Df	tex;
		};

		struct DecalColorVertex
		{
			Vector3Df	pos;
			Color4		color;
			Vector2Df	tex;
		};
	}
}

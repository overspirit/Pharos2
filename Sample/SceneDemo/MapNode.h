#pragma once

namespace Pharos
{
	namespace Scene
	{
		class MapNode : public SceneNode
		{
		public:
			MapNode();
			virtual ~MapNode();

		private:
			struct Vertex
			{
				Vector3Df pos;
				Color4	color;
			};

		private:
			vector<Vector3Df>		m_regionList;

		public:
			bool LoadMap(const char8* path);

			const Vector3Df& GetSubRegion(uint32 index) { return m_regionList[0]; }
		};
	}
}
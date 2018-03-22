#pragma once

namespace Pharos
{
	namespace Scene
	{
		class GridNode : public SceneNode
		{
		public:
			GridNode();
			virtual ~GridNode();

		private:
			struct Vertex
			{
				Vector3Df pos;
				Color4	color;
			};

		private:
			Model*		m_gridModel;

		public:
			virtual bool BuildGridData();
		};
	}
}
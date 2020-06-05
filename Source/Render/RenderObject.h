#pragma once

namespace Pharos
{
	namespace Render
	{
		class RenderObject
		{
		public:
			RenderObject();
			virtual ~RenderObject();

		private:
			RenderBuffer*           		m_vertBuf;
			RenderBuffer*           		m_indexBuf;
			RenderPipeline*					m_pipeline;
			vector<RenderTexture*>			m_texList;
			vector<RenderBuffer*>           m_uniformBufs;
			
		private:

		public:
			virtual void Draw();
		};
	}
}

#pragma once

namespace Pharos
{
	namespace Render
	{
		class RenderObject
		{
		public:
			RenderObject(RenderCommand* renderCmd);
			virtual ~RenderObject();

		private:
			struct RenderBlock
			{
				RenderBuffer*           		vertBuf;
				RenderBuffer*           		indexBuf;
				RenderResourceSet*				resSet;
				RenderPipeline*					pipeline;

				DrawType	drawType;
				uint32		itemCount;
			};


		private:
			RenderCommand*					m_renderCmd;

			vector<RenderBlock>				m_blockList;

		public:
			virtual uint32 AddRenderBlock(RenderBuffer* vertBuf, RenderPipeline* pipeline);
			virtual void SetBlockDrawInfo(uint32 blockIndex, DrawType drawType, uint32 itemCount);
			virtual void SetBlockIndexBuffer(uint32 blockIndex, RenderBuffer* indexBuffer);
			virtual void SetBlockResourceSet(uint32 blockIndex, RenderResourceSet* resSet);			
			
			virtual void Draw();
		};
	}
}

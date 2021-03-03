#pragma once

namespace Pharos
{
	namespace Render
	{
		class RenderObject
		{
        private:
			RenderObject(RenderCommand* renderCmd);
			virtual ~RenderObject();

            friend class RenderMgr;
            
		private:
			struct RenderBlock
			{
				RenderBuffer*           		vertBuf;
				RenderBuffer*           		indexBuf;
				RenderResourceSet*				resSet;
				RenderPipeline*					pipeline;

				DrawType	drawType;
				uint32		itemCount;
				uint32		itemOffset;
			};


		private:
			RenderCommand*					m_renderCmd;

			vector<RenderBlock>				m_blockList;

		public:
			virtual uint32 AddRenderBlock(RenderBuffer* vertBuf, RenderPipeline* pipeline);
			virtual void SetBlockDrawInfo(uint32 blockIndex, DrawType drawType, uint32 itemCount, uint32 itemOffset = 0);
			virtual void SetBlockIndexBuffer(uint32 blockIndex, RenderBuffer* indexBuffer);
			virtual void SetBlockResourceSet(uint32 blockIndex, RenderResourceSet* resSet);			
			
			virtual void Draw();
		};
	}
}

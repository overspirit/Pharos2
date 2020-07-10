#pragma once

namespace Pharos
{
    namespace Render
    {
		enum IndexElementType
		{
			IET_UINT32,
			IET_UINT16,
		};
		
        class RenderCommand
        {
        public:
            virtual ~RenderCommand(){}
            
        public:
			virtual void SetDebugLabel(const char8* label) = 0;

            virtual void BeginCommand() = 0;
			
            virtual void SetVertexBuffer(RenderBuffer* buffer) = 0;		
			virtual void SetIndexBuffer(RenderBuffer* indexBuffer, IndexElementType indexType) = 0;			
            
            virtual void SetViewport(const Rect2Di& viewRect, float32 minDepth, float32 maxDepth) = 0;
            virtual void SetScissorRect(const Rect2Di& scissorRect) = 0;

            virtual void SetPipeline(RenderResourceSet* resSet, RenderPipeline* pipeline) = 0;
            
			virtual void DrawPrimitives(uint32 start, uint32 count) = 0;
			virtual void DrawIndexedPrimitives(uint32 start, uint32 count) = 0;
            
            virtual void EndCommand() = 0;
        };
    }
}

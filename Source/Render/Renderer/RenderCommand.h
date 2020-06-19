#pragma once

namespace Pharos
{
    namespace Render
    {
        class RenderCommand
        {
        public:
            virtual ~RenderCommand(){}
            
        public:
			virtual void SetDebugLabel(const char8* label) = 0;

            virtual void BeginCommand() = 0;
			
            virtual void SetVertexBuffer(uint32 slot, RenderBuffer* buffer) = 0;
            virtual void SetFragmentBuffer(uint32 slot, RenderBuffer* buffer) = 0;
            virtual void SetFragmentTexture(uint32 slot, RenderTexture* tex) = 0;
            
            virtual void SetViewport(const Rect2Di& viewRect, float32 minDepth, float32 maxDepth) = 0;
            virtual void SetScissorRect(const Rect2Di& scissorRect) = 0;

            virtual void SetPipeline(RenderPipeline* pipeline) = 0;
            
            virtual void DrawImmediate(DrawType type, uint32 start, uint32 count) = 0;
            
            virtual void EndCommand() = 0;
        };
    }
}

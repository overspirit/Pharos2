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
			
            virtual void SetVertexBuffer(uint32 slot, RenderBuffer* buffer) = 0;
            virtual void SetFragmentBuffer(uint32 slot, RenderBuffer* buffer) = 0;
            virtual void SetFragmentTexture(uint32 slot, RenderTexture* tex) = 0;
            
            virtual void SetPipeline(RenderPipeline* pipeline) = 0;
            
            virtual void DrawImmediate(DrawType type, uint32 start, uint32 count) = 0;
            
            virtual void EndCommand() = 0;
        };
    }
}

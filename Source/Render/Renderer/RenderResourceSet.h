#pragma once

namespace Pharos
{
    namespace Render
    {
        class RenderResourceSet
        {
        public:
            virtual ~RenderResourceSet(){}
            
        public:			
            virtual void SetVertexUniformBuffer(uint32 slot, RenderBuffer* buffer) = 0;
            virtual void SetFragmentUniformBuffer(uint32 slot, RenderBuffer* buffer) = 0;

            virtual void SetVertexTexture(uint32 slot, RenderTexture* tex) = 0;
            virtual void SetFragmentTexture(uint32 slot, RenderTexture* tex) = 0;

            virtual bool UpdateSet() = 0;
        };
    }
}

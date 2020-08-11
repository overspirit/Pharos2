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
            virtual bool SetVertexUniformBuffer(uint32 slot, RenderBuffer* buffer) = 0;
            virtual bool SetFragmentUniformBuffer(uint32 slot, RenderBuffer* buffer) = 0;

            virtual bool SetVertexTexture(uint32 slot, RenderTexture* tex) = 0;
            virtual bool SetFragmentTexture(uint32 slot, RenderTexture* tex) = 0;

            virtual void UpdateSet() = 0;
        };
    }
}

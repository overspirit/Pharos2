#pragma once

namespace Pharos
{
	namespace Render
	{
		class MetalRenderTarget : public RenderTarget
		{
		public:
			MetalRenderTarget(id<MTLDevice> device, int32 width, int32 height);
			virtual ~MetalRenderTarget(void);

		protected:
			id<MTLDevice>				m_device;
			
			MTLRenderPassDescriptor*	m_passDesc;
			
			MetalRenderTexture*				m_colorAttach[8];
			MetalRenderTexture*				m_depthStencilAttach;
			
		public:
			virtual MTLRenderPassDescriptor* GetMetalPassDescriptor() { return m_passDesc; }
			
            virtual EPixelFormat GetColorAttachFormat(uint32 slot);
            virtual EPixelFormat GetDepthAttachFormat();
            
		public:
			virtual void SetClear(Color4 color = 0xFF000000, float32 depth = 1.0f, uint32 stencil = 0);

            virtual void SetColorAttach(uint32 slot, RenderTexture* tex);
            virtual RenderTexture* GetColorAttachTexture(uint32 slot);

            virtual void SetDepthStencilAttach(RenderTexture* tex);
            virtual RenderTexture* GetDepthStencilAttachTexture();
		};
	}
}

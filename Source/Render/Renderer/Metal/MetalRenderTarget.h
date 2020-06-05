#pragma once

namespace Pharos
{
	namespace Render
	{
		class MetalRenderTarget : public RenderTarget
		{
		public:
			MetalRenderTarget(id<MTLDevice> device);
			virtual ~MetalRenderTarget(void);

		protected:
			id<MTLDevice>				m_device;
			
			MTLRenderPassDescriptor*	m_passDesc;
			
			MetalRenderTexture*				m_colorAttach[8];
			MetalRenderTexture*				m_depthAttach;
			MetalRenderTexture*				m_stencilAttach;
			
		public:
			virtual MTLRenderPassDescriptor* GetMetalPassDescriptor() { return m_passDesc; }
			
			virtual bool InitRenderPass(int32 width, int32 height);

		public:
			virtual void SetClear(Color4 color = 0xFF000000, float32 depth = 1.0f, uint32 stencil = 0);

			virtual RenderTexture* GenerateColorAttach(uint32 slot, EPixelFormat fmt);
			virtual void SetColorAttach(uint32 slot, RenderTexture* tex);
			virtual RenderTexture* GenerateDepthAttach(EPixelFormat fmt);
			virtual void SetDepthAttach(RenderTexture* tex);
			virtual RenderTexture* GenerateStencilAttach(EPixelFormat fmt);
			virtual void SetStencilAttach(RenderTexture* tex);
			
			virtual RenderTexture* GetColorAttachTexture(uint32 slot) { return m_colorAttach[slot]; }
			virtual RenderTexture* GetDepthAttachTexture() { return m_depthAttach; }
			virtual RenderTexture* GetStencilAttachTexture() { return m_stencilAttach; }

			virtual EPixelFormat GetColorAttachFormat(uint32 slot);
			virtual EPixelFormat GetDepthAttachFormat();
			virtual EPixelFormat GetStencilAttachFormat();
		};
	}
}

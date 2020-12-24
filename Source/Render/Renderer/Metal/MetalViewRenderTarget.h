#pragma once

namespace Pharos
{
	namespace Render
	{
		/*
		 //MetalViewRenderTarget 是为了解决Metal渲染中，最终的渲染目标由MTKView持有
		 //在MTKView 调用MTKViewDelegate的draw之后，最终渲染目标会切换(CMetalLayer 的 nextDrawable)
		 //所以在调用GetMetalPassDescriptor会使用MTKView的currRenderPass重新获取RenderPass
		 */
		class MetalViewRenderTarget : public MetalRenderTarget
		{
		public:
			MetalViewRenderTarget(id<MTLDevice> device, MTKView* view);
			virtual ~MetalViewRenderTarget(void);

		private:
			MTKView*					m_view;
			
		public:
			virtual MTLRenderPassDescriptor* GetMetalPassDescriptor();
			
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

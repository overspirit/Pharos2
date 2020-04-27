#pragma once

namespace Pharos
{
	namespace Render
	{
		class MetalRenderer : public Renderer
		{
		public:
			MetalRenderer(void);
			virtual ~MetalRenderer(void);

		private:
			MTKView*     m_view;

			id<MTLDevice>           m_device;
			MTLRenderPassDescriptor*    m_renderPass;

			id<MTLCommandQueue>         m_commandQueue;

			//            MetalFrameBuffer*        m_defFrameBuf;

			MetalRenderPipeline*        m_pipeline;
			//MetalRenderPass*            m_pass;

		public:
			virtual bool Init();
			virtual bool Create(const DeviceConfig& cfg);
			virtual void Destroy();

			//virtual RenderFrameBuffer* GetDefaultFrameBuffer() const { return m_defFrameBuf; }
			virtual const char8* GetAdapterName() const { return NULL; }
			virtual uint32 GetAdapterMemorySize() const { return 0; }

			virtual RenderBuffer* GenerateRenderBuffer(uint32 bufSize, MemoryBuffer* buf = nullptr);
			virtual RenderTexture* CreateTexture(int32 width, int32 height, EPixelFormat fmt);
			virtual RenderTexture* LoadTexture(const char8* szPath);
			virtual RenderTexture* LoadTexture(const Image* image);

			virtual RenderProgram* GenerateRenderProgram(const char8* libPath);
			virtual RenderTarget* CreateRenderTarget(int32 width, int32 height);
			virtual RenderSamplerState* CreateSampleState(const SamplerStateDesc& desc);
			virtual RenderBlendState* CreateBlendState(const BlendStateDesc& desc);
			virtual RenderRasterizerState* CreateRasterizerState(const RasterizerStateDesc& desc);
			virtual RenderDepthStencilState* CreateDepthStencilState(const DepthStencilStateDesc& desc);
			virtual RenderPipeline* GenerateRenderPipeline();

			virtual RenderCommand* GenerateRenderCommand();
		};
	}
}

#pragma once

namespace Pharos
{
	namespace Render
	{
		class MetalRenderer : public Renderer, public IMetalCommandFactory
		{
		public:
			MetalRenderer(void);
			virtual ~MetalRenderer(void);

		private:
			MTKView*     m_view;

			id<MTLDevice>           m_device;
			MTLRenderPassDescriptor*    m_renderPass;

			id<MTLCommandQueue>         	m_commandQueue;
			id<MTLCommandBuffer>			m_currCmdBuffer;
			
			MetalRenderTarget*			m_defaultTarget;
		
		public:
			virtual id<MTLCommandBuffer> MakeMetalCommandBuffer();
			
		public:
			virtual bool Init();
			virtual bool Create(const DeviceConfig& cfg);
			virtual void Destroy();
			
			virtual void Commit();

			virtual RenderTarget* GetDefaultRenderTarget() { return m_defaultTarget; }
			virtual const char8* GetAdapterName() const { return NULL; }
			virtual uint32 GetAdapterMemorySize() const { return 0; }

			virtual RenderBuffer* GenerateRenderBuffer(BufferType type);
			virtual RenderTexture* CreateTexture(int32 width, int32 height, EPixelFormat fmt);
			virtual RenderTexture* LoadTexture(const char8* szPath);
			virtual RenderTexture* LoadTexture(const Image* image);

			virtual RenderProgram* GenerateRenderProgram();
			virtual RenderTarget* CreateRenderTarget(int32 width, int32 height);
			virtual RenderSamplerState* CreateSampleState(const SamplerStateDesc& desc);
			virtual RenderBlendState* CreateBlendState(const BlendStateDesc& desc);
			virtual RenderRasterizerState* CreateRasterizerState(const RasterizerStateDesc& desc);
			virtual RenderDepthStencilState* CreateDepthStencilState(const DepthStencilStateDesc& desc);
			virtual RenderResourceSet* GenerateRenderResuourceSet();
			virtual RenderPipeline* GenerateRenderPipeline();

			virtual RenderCommand* GenerateRenderCommand(RenderTarget* renderTarget);
		};
	}
}

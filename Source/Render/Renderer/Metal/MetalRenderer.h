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

		public:
			virtual bool Init();
			virtual bool Create(const DeviceConfig& cfg);
			virtual void Destroy();

			virtual RenderFrameBuffer* GetDefaultFrameBuffer() const { return NULL; }
            virtual const char8* GetAdapterName() const { return NULL; }
			virtual uint32 GetAdapterMemorySize() const { return 0; }

			virtual RenderProgram* GenerateRenderProgram();
			virtual RenderLayout* GenerateRenderLayout(uint32 vertSize, MemoryBuffer* buf = nullptr);
			virtual RenderTexture* CreateTexture(int32 width, int32 height, EPixelFormat fmt);
			virtual RenderTexture* CreateTargetTexture(int32 width, int32 height, EPixelFormat fmt);
			virtual RenderTexture* CreateDepthTexture(int32 width, int32 height);
			virtual RenderTexture* LoadTexture(const char8* szPath);
			virtual RenderTexture* LoadTexture(const Image* image);
			virtual RenderShaderData* CreateShaderData();
			virtual RenderFrameBuffer* CreateFrameBuffer(int32 width, int32 height);
			virtual RenderSamplerState* CreateSampleState(const SamplerStateDesc& desc);
			virtual RenderBlendState* CreateBlendState(const BlendStateDesc& desc);
			virtual RenderRasterizerState* CreateRasterizerState(const RasterizerStateDesc& desc);
			virtual RenderDepthStencilState* CreateDepthStencilState(const DepthStencilStateDesc& desc);		

			virtual void BindFrameBuffer(RenderFrameBuffer* frameBuf);
			virtual void BindLayout(RenderLayout* layout);
			virtual void BindShaderData(uint32 slot, RenderShaderData* data);
			virtual void BindTexture(uint32 slot, RenderTexture* tex);
			virtual void BindProgram(RenderProgram* program);
			virtual void BindBlendState(RenderBlendState* state);
			virtual void BindRasterizerState(RenderRasterizerState* state);
			virtual void BindDepthStencilState(RenderDepthStencilState* state);

			virtual void DrawImmediate(DrawType type, uint32 start, uint32 count);

			virtual bool Present();
		};
	}
}

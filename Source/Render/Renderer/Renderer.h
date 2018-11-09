#pragma once

namespace Pharos
{
	namespace Render
	{
        struct DeviceConfig
        {
            const void*         wndHandle;
            int32				width;
            int32				height;
            EMultisampleType	sampleType;
            bool				sync;
            bool				fullScreen;
            EPixelFormat        bufferFormat;
            EPixelFormat        depthFormat;
            uint32              refreshRate;
        };
        
		class Renderer 
		{
		public:
			Renderer(void)
			{

			}

			virtual ~Renderer(void)
			{

			}

		protected:

		public:
			virtual bool Init() = 0;
			virtual bool Create(const DeviceConfig& cfg) = 0;
			virtual void Destroy() = 0;			

			virtual RenderFrameBuffer* GetDefaultFrameBuffer() const = 0;
			virtual const char8* GetAdapterName() const = 0;
			virtual uint32 GetAdapterMemorySize() const = 0;

			virtual RenderProgram* GenerateRenderProgram() = 0;
			virtual RenderLayout* GenerateRenderLayout(uint32 vertSize, MemoryBuffer* buf = nullptr) = 0;
			virtual RenderTexture* CreateTexture(int32 width, int32 height, EPixelFormat fmt) = 0;
			virtual RenderTexture* LoadTexture(const char8* szPath) = 0;
			virtual RenderTexture* LoadTexture(const Image* image) = 0;
			virtual RenderShaderData* CreateShaderData() = 0;
			virtual RenderFrameBuffer* CreateFrameBuffer(int32 width, int32 height) = 0;
			virtual RenderSamplerState* CreateSampleState(const SamplerStateDesc& desc) = 0;
			virtual RenderBlendState* CreateBlendState(const BlendStateDesc& desc) = 0;
			virtual RenderRasterizerState* CreateRasterizerState(const RasterizerStateDesc& desc) = 0;
			virtual RenderDepthStencilState* CreateDepthStencilState(const DepthStencilStateDesc& desc) = 0;

			virtual void BindFrameBuffer(RenderFrameBuffer* frameBuf) = 0;
			virtual void BindLayout(RenderLayout* layout) = 0;
			virtual void BindShaderData(uint32 slot, RenderShaderData* data) = 0;
			virtual void BindTexture(uint32 slot, RenderTexture* tex) = 0;
			virtual void BindProgram(RenderProgram* program) = 0;
			virtual void BindBlendState(RenderBlendState* state) = 0;
			virtual void BindRasterizerState(RenderRasterizerState* state) = 0;
			virtual void BindDepthStencilState(RenderDepthStencilState* state) = 0;

			virtual void DrawImmediate(DrawType type, uint32 start, uint32 count) = 0;

			virtual bool Present() = 0;
		};
	}
}


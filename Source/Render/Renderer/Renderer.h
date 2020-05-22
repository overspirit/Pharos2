#pragma once

namespace Pharos
{
	namespace Render
	{
        struct DeviceConfig
        {
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
			virtual ~Renderer(void)
			{
				
			}

		protected:

		public:
			virtual bool Init() = 0;
			virtual bool Create(const DeviceConfig& cfg) = 0;
			virtual void Destroy() = 0;
			
			virtual void Commit() = 0;
			
			virtual RenderTarget* GetDefaultRenderTarget() = 0;
			virtual const char8* GetAdapterName() const = 0;
			virtual uint32 GetAdapterMemorySize() const = 0;
            
			virtual RenderBuffer* GenerateRenderBuffer(uint32 bufSize, MemoryBuffer* buf = nullptr) = 0;
			virtual RenderTexture* CreateTexture(int32 width, int32 height, EPixelFormat fmt) = 0;
			virtual RenderTexture* LoadTexture(const char8* szPath) = 0;
			virtual RenderTexture* LoadTexture(const Image* image) = 0;
            
            virtual RenderProgram* GenerateRenderProgram() = 0;
			virtual RenderTarget* CreateRenderTarget(int32 width, int32 height) = 0;
			virtual RenderSamplerState* CreateSampleState(const SamplerStateDesc& desc) = 0;
			virtual RenderBlendState* CreateBlendState(const BlendStateDesc& desc) = 0;
			virtual RenderRasterizerState* CreateRasterizerState(const RasterizerStateDesc& desc) = 0;
			virtual RenderDepthStencilState* CreateDepthStencilState(const DepthStencilStateDesc& desc) = 0;
            virtual RenderPipeline* GenerateRenderPipeline() = 0;
            
            virtual RenderCommand* GenerateRenderCommand(RenderTarget* renderTarget) = 0;
		};
	}
}


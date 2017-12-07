#pragma once

namespace Pharos
{
	namespace Render
	{
		class D3D11Renderer : public Renderer
		{
		public:
			D3D11Renderer(void);
			virtual ~D3D11Renderer(void);

		private:
			D3D_DRIVER_TYPE		m_driverType;
			D3D_FEATURE_LEVEL	m_featureLevel;

			DXGI_SWAP_CHAIN_DESC		m_swapChainDesc;

			ID3D11Device*			m_device;
			ID3D11DeviceContext*	m_context;

			IDXGIAdapter*	m_dxgiAdapter;
			IDXGIFactory*	m_dxgiFactory;
			IDXGISwapChain* m_swapChain;

			D3D11FrameBuffer*		m_defFrameBuf;
			string					m_adapterName;
			uint32					m_adapterMemSize;

			D3D11RenderLayout*		m_bindLayout;
			D3D11ShaderProgram*		m_bindShader;

		public:
			virtual bool Init();
			virtual bool Create(const DeviceConfig& cfg);
			virtual void Destroy();

			virtual RenderFrameBuffer* GetDefaultFrameBuffer() const { return m_defFrameBuf; }
			virtual const char8* GetAdapterName() const { return m_adapterName.c_str(); }
			virtual uint32 GetAdapterMemorySize() const { return m_adapterMemSize; }

			virtual RenderProgram* GenerateRenderProgram();
			virtual RenderLayout* GenerateRenderLayout(uint32 vertSize, MemoryBuffer* buf = nullptr);
			virtual RenderTexture* CreateTexture(int32 width, int32 height, EPixelFormat fmt);
			virtual RenderTexture* LoadTexture(const char8* szPath);
			virtual RenderTexture* LoadTexture(const Image& image);
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
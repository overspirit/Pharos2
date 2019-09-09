#pragma once

namespace Pharos
{
	namespace Render
	{
		class D3D11FrameBuffer : public RenderFrameBuffer
		{
		public:
			D3D11FrameBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
			virtual ~D3D11FrameBuffer(void);

		private:
			ID3D11Device*			m_pDevice;
			ID3D11DeviceContext*	m_pContext;

			ID3D11RenderTargetView*		m_pTargetSlots[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
			D3D11Texture*				m_pTargetTexs[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
			ID3D11DepthStencilView*		m_pDepthView;
			D3D11Texture*				m_pDepthTex;

		public:
			virtual bool InitFrameBuffer(int32 width, int32 height);
			virtual bool GenerateDefaultFrameBuffer(IDXGISwapChain* pSwapChain);
			
			virtual void ClearFrameBuffer(Color4 color = 0xFF000000, float32 depth = 1.0f, uint32 stencil = 0);
			
			virtual RenderTexture* CreateRenderTexture(uint32 slot, EPixelFormat fmt);
			virtual RenderTexture* CreateDepthTexture();

			virtual RenderTexture* GetRenderTexture(uint32 slot);
			virtual RenderTexture* GetDepthTexture();

			virtual void AttachTexture(uint32 slot, RenderTexture* tex);

			virtual void ApplyDevice();
		};
	}
}

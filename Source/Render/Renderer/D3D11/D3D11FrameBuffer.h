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

			int32		m_width;
			int32		m_height;

			Rect2Di		m_viewPort;

			ID3D11RenderTargetView*		m_pTargetSlots[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
			D3D11Texture*				m_pTargetTexs[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
			ID3D11DepthStencilView*		m_pDepthView;
			D3D11Texture*				m_pDepthTex;

		public:
			virtual bool InitFrameBuffer(int32 width, int32 height);
			virtual bool GenerateDefaultFrameBuffer(IDXGISwapChain* pSwapChain);
			
			virtual void ClearFrameBuffer(Color4 color = 0xFF000000, float32 depth = 1.0f, uint32 stencil = 0);

			virtual const Rect2Di& GetViewport() const { return m_viewPort; }
			virtual void SetViewPort(const Rect2Di& rect) { m_viewPort = rect; }

			virtual const RenderTexture& CreateRenderTexture(uint32 slot, EPixelFormat fmt);
			virtual const RenderTexture& CreateDepthTexture();

			virtual const RenderTexture& GetRenderTexture(uint32 slot);
			virtual const RenderTexture& GetDepthTexture();

			virtual int32 GetWidth() { return m_width; }
			virtual int32 GetHeight() { return m_height; }

			virtual void ApplyDevice();
		};
	}
}

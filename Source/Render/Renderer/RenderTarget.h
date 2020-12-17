#pragma once

namespace Pharos
{
	namespace Render
	{
		class RenderTarget
		{
		public:
			RenderTarget()
			{
				m_width = 0;
				m_height = 0;
			}
			
			virtual ~RenderTarget() {}

		protected:
			int32			m_width;
			int32			m_height;

			Rect2Di		m_viewPort;

		public:
			virtual void SetClear(Color4 color, float32 depth = 1.0f, uint32 stencil = 0) = 0;

			virtual const Rect2Di& GetViewport() const { return m_viewPort; }
			virtual void SetViewPort(const Rect2Di& rect) { m_viewPort = rect; }

			virtual void SetColorAttach(uint32 slot, RenderTexture* tex) = 0;
			virtual RenderTexture* GetColorAttachTexture(uint32 slot) = 0;

			virtual void SetDepthStencilAttach(RenderTexture* tex) = 0;			
			virtual RenderTexture* GetDepthStencilAttachTexture() = 0;
			
			virtual int32 GetWidth() { return m_width; }
			virtual int32 GetHeight() { return m_height; }
		};
	}
}

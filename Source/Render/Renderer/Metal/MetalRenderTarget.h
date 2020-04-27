#pragma once

namespace Pharos
{
	namespace Render
	{
		class MetalRenderTarget //: public RenderTarget
		{
		public:
			MetalRenderTarget();
			virtual ~MetalRenderTarget(void);

		private:

		public:
			virtual bool Init(int32 width, int32 height);

			virtual void Clear(Color4 color = 0xFF000000, float32 depth = 1.0f, uint32 stencil = 0);

			virtual RenderTexture* CreateRenderTexture(uint32 slot, EPixelFormat fmt);
			virtual RenderTexture* CreateDepthTexture();

			virtual RenderTexture* GetRenderTexture(uint32 slot);
			virtual RenderTexture* GetDepthTexture();

			virtual void AttachTexture(uint32 slot, RenderTexture* tex);

			virtual void ApplyDevice();
		};
	}
}

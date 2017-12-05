#pragma once

namespace Pharos
{
	namespace Render
	{
		class RenderFrameBuffer
		{
		public:
			virtual ~RenderFrameBuffer(){}

		public:
			virtual void ClearFrameBuffer(Color4 color = 0xFF000000, float32 depth = 1.0f, uint32 stencil = 0) = 0;

			virtual const Rect2Di& GetViewport() const = 0;
			virtual void SetViewPort(const Rect2Di& rect) = 0;

			virtual const RenderTexture& CreateRenderTexture(uint32 slot, EPixelFormat fmt) = 0;
			virtual const RenderTexture& CreateDepthTexture() = 0;

			virtual const RenderTexture& GetRenderTexture(uint32 slot) = 0;
			virtual const RenderTexture& GetDepthTexture() = 0;

			virtual int32 GetWidth() = 0;
			virtual int32 GetHeight() = 0;
		};
	}
}
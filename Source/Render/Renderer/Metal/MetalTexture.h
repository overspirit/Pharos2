#pragma once

namespace Pharos
{
	namespace Render
	{
		class MetalTexture : public RenderTexture
		{
		public:
			MetalTexture(id<MTLDevice> device);
			virtual ~MetalTexture();

		protected:
			id<MTLDevice>   m_device;
			id<MTLTexture>  m_texture;

		public:
			id<MTLTexture> GetMetalTexture() { return m_texture; }

		public:
			virtual bool Create(int32 width, int32 height, EPixelFormat fmt);


		public:
			virtual bool LoadFromFile(const char8* szPath);
			virtual bool LoadFromImage(const Image* pImage);

			virtual bool CopyFromData(const void* pImageData, uint32 nDataSize);
			virtual bool CopyRectFromData(const void* pData, uint32 nDataSize, const Rect2Di& rt);
			virtual bool CopyFromTexture(RenderTexture* srcTex);
			virtual bool CopyRectFromTexture(RenderTexture* srcTex, const Rect2Di& srcRect, const Rect2Di& destRect);

			virtual void SetSampleState(RenderSamplerState* state);

			virtual uint32 GetWidth() const { return m_width; }
			virtual uint32 GetHeight() const { return m_height; }
			virtual EPixelFormat GetFormat() const { return m_fmt; }

			virtual void ApplyDevice(uint32 slot);
		};
	}
}

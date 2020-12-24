#pragma once

namespace Pharos
{
	namespace Render
	{
		class MetalRenderTexture : public RenderTexture
		{
		public:
			MetalRenderTexture(id<MTLDevice> device);
			virtual ~MetalRenderTexture();

		protected:
			id<MTLDevice>   m_device;
			id<MTLTexture>  m_texture;

		public:
			id<MTLTexture> GetMetalTexture() { return m_texture; }
			
		public:
			virtual bool Create(int32 width, int32 height, EPixelFormat fmt);
			virtual bool CreateColorAttach(int32 width, int32 height, EPixelFormat fmt);
			virtual bool CreateDepthAttach(int32 width, int32 height, EPixelFormat fmt);

			virtual bool LoadFromFile(const char8* szPath);
			virtual bool LoadFromImage(const Image* pImage);
            
            virtual bool Save(const char8* path);

			virtual bool CopyFromData(const void* pImageData, uint32 nDataSize);
			virtual bool CopyRectFromData(const void* pData, uint32 nDataSize, const Rect2Di& rt);
			virtual bool CopyFromTexture(RenderTexture* srcTex);
			virtual bool CopyRectFromTexture(RenderTexture* srcTex, const Rect2Di& srcRect, const Rect2Di& destRect);

			virtual void SetSampleState(const SamplerStateDesc& state);

			virtual uint32 GetWidth() const { return m_width; }
			virtual uint32 GetHeight() const { return m_height; }
			virtual EPixelFormat GetFormat() const { return m_fmt; }

			virtual void ApplyVertexTexture(uint32 slot, id<MTLRenderCommandEncoder> encoder);
			virtual void ApplyFragmentTexture(uint32 slot, id<MTLRenderCommandEncoder> encoder);
		};
	}
}

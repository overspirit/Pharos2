#pragma once

namespace Pharos
{
	namespace Render
	{
		class RenderTexture
		{
		public:
			virtual ~RenderTexture(){}

		protected:
			uint32			m_width;
			uint32			m_height;
			EPixelFormat	m_fmt;

		public:
			virtual void SetSampleState(RenderSamplerState* state) = 0;

			//从内存数据上更新纹理
			virtual bool CopyFromData(const void* pImageData, uint32 nDataSize) = 0;
			virtual bool CopyRectFromData(const void* pData, uint32 nDataSize, const Rect2Di& rt) = 0;

			//从另一个纹理数据上更新纹理
			virtual bool CopyFromTexture(RenderTexture* srcTex) = 0;
			virtual bool CopyRectFromTexture(RenderTexture* srcTex, const Rect2Di& srcRect, const Rect2Di& destRect) = 0;

			//得到贴图对象占用的内存的大小
			virtual uint32 GetWidth() const { return m_width; }
			virtual uint32 GetHeight() const { return m_height; }
			virtual EPixelFormat GetFormat() const { return m_fmt; }
		};
	}
}
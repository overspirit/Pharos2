#pragma once

namespace Pharos
{
	namespace Desktop
	{
		class RenderImage
		{
		public:
			RenderImage(void);
			virtual ~RenderImage(void);

		private:
			Image*		m_image;

			RenderTexture*		m_imageTex;

			RenderTechnique*	m_imageTech;
			RenderVariable*		m_imageTexVar;

			RenderTechnique*	m_grayImageTech;
			RenderVariable*		m_grayImageTexVar;

		public:
			virtual bool LoadImage(const char8* imageFilePath);

			virtual void RenderImageRect(const Rect2Di& imageRect, Color4 imageColor, const Rect2Di& drawRect);
			virtual void RenderGrayImageRect(const Rect2Di& imageRect, const Rect2Di& drawRect);
		};
	}
}

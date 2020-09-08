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
			struct DrawUniform
			{
				float32 round;
				float32 scale;
			};

		private:
			Image*		m_image;
			Color4      m_color;

			RenderTexture*			m_imageTex;
			RenderBuffer*			m_drawUniform;
			RenderResourceSet*		m_resSet;

		public:
		    virtual bool Init();

			virtual bool LoadImage(const char8* imageFilePath);
			virtual void SetImageColor(Color4 color) { m_color = color; }

			virtual void RenderRect(const Rect2Di& imageRect, const Rect2Di& drawRect, float32 round);
			virtual void RenderGrayRect(const Rect2Di& imageRect, const Rect2Di& drawRect, float32 round);
		};
	}
}

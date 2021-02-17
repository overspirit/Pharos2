#pragma once

namespace Pharos
{
	namespace Desktop
	{
		class Texture : public UIObject
		{
		public:
			Texture(void);
			virtual ~Texture(void);

			DECLARE_UI_CLASS(Texture);

		private:
			RenderImage*		m_renderImage;

			Color4				m_imageColor;
			Rect2Di				m_imageRect;
			float32 			m_round;

		public:
			virtual bool LoadFromXml(XmlNode* xmlNode);

            virtual void DrawRect(const Rect2Di& rect);

			virtual void Update(float32 fElapsed);
		};
	}
}

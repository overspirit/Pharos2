#pragma once

namespace Pharos
{
	namespace Desktop
	{
		class FontString : public UIObject
		{
		public:
			FontString(void);
			virtual ~FontString(void);

			DECLARE_UI_CLASS(FontString);

		protected:
			RenderFont*		m_renderFont;
			string			m_showText;

		public:
			virtual bool LoadFromXml(XmlNode* xmlNode);

			virtual void SetText(const char8* text) { m_showText = text; }

			virtual void Update(float32 fElapsed);
		};
	}
}

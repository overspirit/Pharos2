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
			//SpriteFontPtr		m_fontHandle;
			//SpriteBrushPtr		m_fontBrush;
			TextAlignType	m_texAlign;
			string			m_showText;

		public:
			virtual bool LoadFromXml(XmlNode* xmlNode);

			virtual void Update(float32 fElapsed);
			virtual void Render(float32 fElapsed);
		};
	}

	typedef std::shared_ptr<Desktop::FontString>		FontStringPtr;
}
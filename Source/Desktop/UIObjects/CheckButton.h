#pragma once

namespace Pharos
{
	namespace Desktop
	{
		class CheckButton : public Button
		{
		public:
			CheckButton(void);
			virtual ~CheckButton(void);

			DECLARE_UI_CLASS(CheckButton);

		protected:
			Texture*		m_checkTex;
			bool			m_isCheck;

		protected:
			bool onLeftButtonUp(const tagInputMsg& msg);
			
		public:
			virtual bool LoadFromXml(XmlNode* xmlNode);

			virtual void Update(float32 fElapsed);
		};
	}
}

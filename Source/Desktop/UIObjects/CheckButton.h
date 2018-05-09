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
			TexturePtr		m_checkTex;
			bool			m_isCheck;

		protected:
			virtual void PushEvent(int32 v1, float32 v2);
			
		public:
			virtual bool LoadFromXml(XmlNode* xmlNode);

			virtual void Update(float32 fElapsed);
			virtual void Render(float32 fElapsed);
		};
	}

	typedef std::shared_ptr<Desktop::CheckButton>	CheckButtonPtr;
}
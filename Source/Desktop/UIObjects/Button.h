#pragma once

namespace Pharos
{
	namespace Desktop
	{
		class Button : public Frame
		{
		public:
			Button(void);
			virtual ~Button(void);

			DECLARE_UI_CLASS(Button);

		protected:
			ButtonState		m_btnState;			//按钮状态
			bool			m_isInsideDown;

			Texture*		m_pushTex;
			Texture*		m_highlightTex;
			Texture*		m_disableTex;
 
			Texture*		m_currTex;
			Color4			m_currColor;

			Point2Di		m_currMouse;

		protected:
			virtual bool onLeftButtonDown(const tagInputMsg& msg);
			virtual bool onLeftButtonUp(const tagInputMsg& msg);
			virtual bool onMouseMove(const tagInputMsg& msg);

			virtual void SetState(ButtonState state);

		public:
			virtual bool LoadFromXml(XmlNode* xmlNode);

			virtual void Update(float32 fElapsed);
			virtual void Render(float32 fElapsed);

			virtual ButtonState GetState(){ return m_btnState; }

			virtual void Enable();
			virtual void Disable();
		};
	}
}

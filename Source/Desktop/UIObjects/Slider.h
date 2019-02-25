#pragma once

namespace Pharos
{
	namespace Desktop
	{
		class Slider : public Frame
		{
		public:
			Slider();
			virtual ~Slider();

			DECLARE_UI_CLASS(Slider);

		protected:
			PutType			m_putType;

 			Texture*		m_thumbBackTex;
			Texture*		m_thumbPushTex;
			Texture*		m_thumbHighlightTex;
			Texture*		m_thumbDisableTex;
 
			Texture*		m_currThumbTex;

			Rect2Di			m_rtThumb;
			Size2Di			m_thumbSize;
			Rect2Di			m_thumbEdge;
			bool			m_bInThumb;
			ButtonState		m_thumbState;

			float32 m_minValue;
			float32 m_maxValue;
			float32 m_step;
			float32 m_currValue;
			float32 m_wheelSpeed;

		protected:
			virtual bool onLeftButtonDown(const tagInputMsg& msg);
			virtual bool onLeftButtonUp(const tagInputMsg& msg);
			virtual bool onMouseMove(const tagInputMsg& msg);

			void SetThumbState(ButtonState state);

		public:
			virtual bool LoadFromXml(XmlNode* xmlNode);

			virtual void SetValueRange(float32 minValue, float32 maxValue, float32 step);
			virtual void SetValue(float32 value);
			virtual float32 GetValue() { return m_currValue; }
			virtual void Update(float32 fElapsed);
			virtual void Render(float32 fElapsed);
		};
	}
}

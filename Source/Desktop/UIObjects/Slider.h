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

 			TexturePtr		m_thumbBackTex;
			TexturePtr		m_thumbPushTex;
			TexturePtr		m_thumbHighlightTex;
			TexturePtr		m_thumbDisableTex;
 
			TexturePtr		m_currThumbTex;

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

			virtual void Update(float32 fElapsed);
			virtual void Render(float32 fElapsed);
		};
	}

	typedef std::shared_ptr<Desktop::Slider>	SliderPtr;
}

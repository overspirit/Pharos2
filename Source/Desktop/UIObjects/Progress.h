#pragma once

namespace Pharos
{
	namespace Desktop
	{
		class Progress : public Frame
		{
		public:
			Progress(void);
			virtual ~Progress(void);

			DECLARE_UI_CLASS(Progress);

		protected:
			TexturePtr			m_progressTex;	//进度条纹理
			Rect2Di				m_progressEdge;
			Rect2Di				m_progressRect;
			PutType				m_putType;		//摆放类型

			float32			m_minValue;	//最小值
			float32			m_maxValue;	//最大值
			float32			m_value;		//值	

		protected:
			bool ReadBar(XmlNode* xmlNode, void* p);

		public:
			virtual bool LoadFromXml(XmlNode* xmlNode);

			virtual void Update(float32 fElapsed);
			virtual void Render(float32 fElapsed);

			virtual float32 GetValue(){ return m_value; }
			virtual void SetValue(float32 fValue);
		};
	}

	typedef std::shared_ptr<Desktop::Progress>	ProgressPtr;
}

#pragma once

namespace Pharos
{
	namespace Desktop
	{
		class WorldFrame : public Frame
		{
		public:
			WorldFrame(void);
			virtual ~WorldFrame(void);

		private:
			Renderer*		m_renderer;
			//RenderFont		m_renderFont;

		public:
			virtual bool Init();

			virtual void SetSize(int32 width, int32 height);	//设置矩形大小

			virtual bool IsVisable(){ return !m_bHidden; }
			virtual bool IsEnable(){ return true; }

			virtual void Update(float32 fElapsed);
			virtual void Render(float32 fElapsed);
		};
	}

	typedef std::shared_ptr<Desktop::WorldFrame>	WorldFramePtr;
}

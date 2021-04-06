#pragma once

namespace Pharos
{
	namespace Core
	{

		class PlatformMacOS : public Utils::Singleton<PlatformMacOS>
		{
		public:
			PlatformMacOS(void);
			virtual ~PlatformMacOS(void);

		private:
			NSWindow*       m_window;
			MTKView*        m_view;
			Timer			m_timer;
			NSTrackingArea* m_trackingArea;
			NSResponder* m_wc;
            
            Size2Di         m_viewSize;

			Point2Di        m_mousePos;
            CODE_STATE      m_mouseState;

		public:
			virtual bool Init();
			virtual void Destroy();
			virtual void Update();

			virtual void onKeyboardEvent(NSEvent* keyEvent);
			virtual void onMouseEvent(NSEvent* mouseEvent);

			virtual void onViewChangeSize(int32 width, int32 height);
		};
	}
}

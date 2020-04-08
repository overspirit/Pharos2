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

		public:
			virtual bool Init();			
			virtual void Destroy();			
			virtual void Update();

			//virtual HWND GetWindowsHandle() { return m_hWnd; }

            //virtual void onKeyboardEvent(const Pharos::KeyEvent& keyEvent);
			//virtual void onMouseEvent(const Pharos::MouseEvent& mouseEvent);

			virtual void onWindowCreate();
			virtual void onWindowChangeSize(int32 width, int32 height);
			virtual void onWindowDestroy();
		};
	}
}

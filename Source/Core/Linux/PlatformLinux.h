#pragma once

namespace Pharos
{
	namespace Core
	{
		class PlatformLinux : public Utils::Singleton<PlatformLinux>
		{
		public:
			PlatformLinux(void);
			virtual ~PlatformLinux(void);

		private:

		private:
			glfw_window*		m_window;

			Point2Di			m_mousePos;

			Timer				m_timer;

		public:
			virtual bool Init();			
			virtual void Destroy();
			
			virtual int32 Run();

			virtual uint32 GetWindowsHandle() { return 0; }

			virtual void onKeyboardEvent(const KeyEvent& keyEvent);
			virtual void onMouseEvent(int32 mouse, int action, int32 posX, int32 posY, int32 wheel);
			virtual void onWindowChangeSize(int32 width, int32 height);
		};
	}
}

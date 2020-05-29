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

		public:
			virtual bool Init();			
			virtual void Destroy();
			
			virtual int32 Run();

			virtual uint32 GetWindowsHandle() { return 0; }

			virtual void onKeyboardEvent(const KeyEvent& keyEvent);
			virtual void onMouseEvent(const MouseEvent& mouseEvent);
			virtual void onWindowChangeSize(int32 width, int32 height);
		};
	}
}

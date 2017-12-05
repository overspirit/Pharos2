#pragma once

namespace Pharos
{
	namespace Core
	{
		class PlatformWindows : public Utils::Singleton<PlatformWindows>
		{
		public:
			PlatformWindows(void);
			virtual ~PlatformWindows(void);

		private:
			HWND			m_hWnd;
			
			Timer			m_timer;

		private: 			
			static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		public:
			virtual bool Init();			
			virtual void Destroy();
			
			virtual int32 Run();

			virtual void onKeyboardEvent(const KeyEvent& keyEvent);
			virtual void onMouseEvent(const MouseEvent& mouseEvent);

			virtual void onWindowCreate();
			virtual void onWindowChangeSize(int32 width, int32 height);
			virtual void onWindowDestroy();
		};
	}
}

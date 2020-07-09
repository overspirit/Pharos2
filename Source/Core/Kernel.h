#pragma once

namespace Pharos
{
	namespace Core
	{
		//???资源的异步机制需要在这里实现
		//???资源要引用，不要每个资源都建立一个实例

		class Kernel : public Utils::Singleton<Kernel>
		{
		public:
			Kernel(void);
			virtual ~Kernel(void);

		private:
			const void*		m_hWnd;
			Size2Di			m_wndSize;

			string			m_homePath;
			string			m_bundlePath;
			
			IApplication*		m_pApp;

		public:
			virtual bool Init(const void* hWnd, int32 wndWidth, int32 wndHeight);
			virtual void Destroy();
			virtual bool StartUp();
			virtual void Run(float32 fElapsed);

			virtual void SetEngineHomePath(const char8* path) { m_homePath = path; }
			virtual void SetEngineBundlePath(const char8* path) { m_bundlePath = path; }
            
			virtual void onKeyboardEvent(const KeyEvent& keyEvent);
			virtual void onMouseEvent(const MouseEvent& mouseEvent);
			virtual void onWindowChangeSize(int32 width, int32 height);

		public:
			virtual IApplication* GetUserApplication() { return m_pApp; }

			virtual const Size2Di& GetWindowSize() const { return m_wndSize; }			
			virtual const void* GetWindowHandle() const { return m_hWnd; }
			virtual const char8* GetHomeDirectoryPath() const { return m_homePath.c_str(); }
			virtual const char8* GetBundleDirectoryPath() const { return m_bundlePath.c_str(); }
		};
	}
}

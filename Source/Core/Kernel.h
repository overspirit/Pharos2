#pragma once

namespace Pharos
{
	namespace Core
	{
		class Kernel : public Utils::Singleton<Kernel>//, public IKernel
		{
		public:
			Kernel(void);
			virtual ~Kernel(void);

		private:
			const void*		m_hWnd;
			Size2Di			m_wndSize;

			string			m_homePath;
			string			m_bundlePath;
			
			FT_Library			m_fontLib;

			IApplication*		m_pApp;

			Vector3Df		m_deviceOrientation;
			Matrix4			m_deviceAttitude;
						
           // ImagePtr        m_captureImage;
            
		public:
			virtual bool Init(const void* hWnd);			
			virtual void Destroy();

			virtual bool StartUp();

			virtual void Update(float32 fElapsed);
			virtual void Render(float32 fElapsed);

			virtual void SetEngineHomePath(const char8* path) { m_homePath = path; }
			virtual void SetEngineBundlePath(const char8* path) { m_bundlePath = path; }

			virtual void SetDeviceOrientation(const Vector3Df& ori) { m_deviceOrientation = ori; }
			virtual void SetDeviceAttitude(const Matrix4& mat) { m_deviceAttitude = mat; }

            virtual void SetCaptureOutput(const void* data, uint32 dataSize);
            
			virtual void onKeyboardEvent(const KeyEvent& keyEvent);
			virtual void onMouseEvent(const MouseEvent& mouseEvent);

			virtual void onViewCreate();
			virtual void onViewChangeSize(int32 width, int32 height);
			virtual void onViewDestroy();

		public:
			virtual IApplication* GetUserApplication() { return m_pApp; }

			virtual const Size2Di& GetWindowSize() const { return m_wndSize; }			
			virtual const void* GetWindowHandle() const { return m_hWnd; }
			virtual const char8* GetHomeDirectoryPath() const { return m_homePath.c_str(); }
			virtual const char8* GetBundleDirectoryPath() const { return m_bundlePath.c_str(); }

			virtual const Vector3Df& GetDeviceOrientation() const { return m_deviceOrientation; }
			virtual const Matrix4& GetHeadTrackerMatrix() const { return m_deviceAttitude; }
			//virtual IImagePtr GetCaptureImage() const { return m_captureImage; }

// 			virtual IMemBufferPtr CreateMemBuffer(uint32 size);
// 			virtual ITimerPtr CreateTimer(bool bStart = false);
// 			virtual IFilePtr CreateFileStream(const char8* path, bool truncate = false);
// 			virtual IFilePtr OpenFileStream(const char8* path);
// 
// 			//!!!改成异步和分包加载资源
// 			virtual IXmlDocumentPtr QueryXmlDocResource(const char8* path);
// 			virtual IXmlDocumentPtr CreateXmlDocResource(const char8* rootName);
// 			virtual IFontPtr QueryFontResource(const char8* path);
// 			virtual IImagePtr QueryImageResource(const char8* path);
// 			virtual IImagePtr CreateImageResource(uint32 width, uint32 height);
// 			virtual IPropertiesPtr QueryPropertiesResource(const char8* path);
		};
	}
}

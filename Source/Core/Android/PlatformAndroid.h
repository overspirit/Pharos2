#pragma once

namespace Pharos
{
	namespace Core
	{
		class PlatformAndroid : public Utils::Singleton<PlatformAndroid>
		{
		public:
			PlatformAndroid(void);
			virtual ~PlatformAndroid(void);

		private:

		private:
            android_app*        m_app;
			AndroidWindow*		m_window;
			Timer				m_timer;

			Point2Di			m_mousePos;

		public:
			virtual bool Init(android_app* app);
			virtual void Destroy();

			virtual int32 Run();

			virtual void onWindowChangeSize(int32 width, int32 height);

			virtual void onMouseEvent(int action, int32 posX, int32 posY, int32 wheel);

			AAssetManager* GetAssetManager() { return m_app->activity->assetManager; }
		};
	}
}

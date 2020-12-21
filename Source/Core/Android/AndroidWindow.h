#pragma once

namespace Pharos
{
	namespace Core
	{
		class AndroidWindow
		{
		public:
			AndroidWindow(ANativeActivity* activity, ANativeWindow*& window, AConfiguration* config);
			virtual ~AndroidWindow();

		private:
            ANativeActivity*    m_activity;
			ANativeWindow*&		m_handle;
            AConfiguration*     m_config;

			bool    m_isFinish;

		public:
			virtual VkSurfaceKHR create_surface(VkInstance instance);

			virtual const char* get_surface_extension();

			virtual bool should_close();
			virtual void close();

			virtual float get_dpi_factor() const;
		};
	}
}
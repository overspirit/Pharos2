#pragma once

namespace Pharos
{
	namespace Core
	{
		class glfw_window : public Utils::Singleton<glfw_window>
		{
		public:
			glfw_window();
			virtual ~glfw_window();

		private:
			GLFWwindow*		m_handle;

		private:
			static void window_size_callback(GLFWwindow *window, int width, int height);
			static void window_focus_callback(GLFWwindow *window, int focused);
			static void key_callback(GLFWwindow *window, int key, int /*scancode*/, int action, int /*mods*/);
			static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
			static void mouse_button_callback(GLFWwindow *window, int button, int action, int /*mods*/);
			static void error_callback(int error, const char *description);
			static void window_close_callback(GLFWwindow *window);

		public:
			virtual bool create_window(const char8 *title, uint32 width, uint32 height);
			virtual VkSurfaceKHR create_surface(VkInstance instance);

			virtual bool should_close();
			virtual void process_events();
			virtual void close();

			float get_dpi_factor() const;
			float get_content_scale_factor() const;
		};

	} // namespace Core
} // namespace Pharos

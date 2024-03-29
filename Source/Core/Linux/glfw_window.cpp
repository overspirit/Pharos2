#include "PreCompile.h"
#include "Pharos.h"


glfw_window::glfw_window()
{
}

glfw_window::~glfw_window()
{
	glfwDestroyWindow(m_handle);
	
	glfwTerminate();
}

bool glfw_window::create_window(const char8* title, uint32 width, uint32 height)
{
	if (!glfwInit()) return false;

	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	m_handle = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!m_handle) return false;

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
	int x = (videoMode->width - width) / 2;
	int y = (videoMode->height - height) / 2;
	glfwSetWindowPos(m_handle, x, y);

	glfwSetWindowUserPointer(m_handle, this);

	glfwSetWindowCloseCallback(m_handle, window_close_callback);
	glfwSetWindowSizeCallback(m_handle, window_size_callback);
	glfwSetWindowFocusCallback(m_handle, window_focus_callback);
	glfwSetKeyCallback(m_handle, key_callback);
	glfwSetCursorPosCallback(m_handle, cursor_position_callback);
	glfwSetMouseButtonCallback(m_handle, mouse_button_callback);
	glfwSetScrollCallback(m_handle, mouse_scroll_callback);

	glfwSetInputMode(m_handle, GLFW_STICKY_KEYS, 1);
	glfwSetInputMode(m_handle, GLFW_STICKY_MOUSE_BUTTONS, 1);

	sPlatform->onWindowChangeSize(width, height);

	return true;
}

VkSurfaceKHR glfw_window::create_surface(VkInstance instance)
{
	VkSurfaceKHR surface;
	VkResult errCode = glfwCreateWindowSurface(instance, m_handle, NULL, &surface);
	if (errCode != VK_SUCCESS)
	{
		return VK_NULL_HANDLE;
	}

	return surface;
}

const char* glfw_window::get_surface_extension()
{
	return VK_KHR_XCB_SURFACE_EXTENSION_NAME;
}

void glfw_window::on_mouse_event(int button, int action, int xpos, int ypos, int wheel)
{
	//printf("on_mouse_event x:%d y:%d button:%d action:%d\n", xpos, ypos, button, action);

	//int width = 0;
	//int height = 0;
	//glfwGetWindowSize(m_handle, &width, &height);
	sPlatform->onMouseEvent(button, action, xpos, ypos, wheel);
}

void glfw_window::on_keyboard_event(int key, int action)
{
	sPlatform->onKeyboardEvent(key, action);
}

void glfw_window::window_close_callback(GLFWwindow *window)
{
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void glfw_window::window_size_callback(GLFWwindow *window, int width, int height)
{
	sPlatform->onWindowChangeSize(width, height);
}

void glfw_window::window_focus_callback(GLFWwindow *window, int focused)
{

}

void glfw_window::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	glfw_window* myWindow = (glfw_window*)glfwGetWindowUserPointer(window);

	myWindow->on_keyboard_event(key, action);
}

void glfw_window::cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
	glfw_window* myWindow = (glfw_window*)glfwGetWindowUserPointer(window);

	myWindow->on_mouse_event(-1, -1, (int32)xpos, (int32)ypos, 0);
}

void glfw_window::mouse_button_callback(GLFWwindow *window, int button, int action, int /*mods*/)
{
	double xpos = 0;
	double ypos = 0;
	glfwGetCursorPos(window, &xpos, &ypos);	

	glfw_window* myWindow = (glfw_window*)glfwGetWindowUserPointer(window);

	myWindow->on_mouse_event(button, action, xpos, ypos, 0);
}

void glfw_window::mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	double xpos = 0;
	double ypos = 0;
	glfwGetCursorPos(window, &xpos, &ypos);	

	glfw_window* myWindow = (glfw_window*)glfwGetWindowUserPointer(window);

	myWindow->on_mouse_event(-1, -1, (int32)xpos, (int32)ypos, yoffset);
}

void glfw_window::error_callback(int error, const char *description)
{
	//LOGE("GLFW Error (code {}): {}", error, description);
}

bool glfw_window::should_close()
{
	return glfwWindowShouldClose(m_handle);
}

void glfw_window::process_events()
{
	glfwPollEvents();
}

void glfw_window::close()
{
	glfwSetWindowShouldClose(m_handle, GLFW_TRUE);
}

float glfw_window::get_dpi_factor() const
{
	auto primary_monitor = glfwGetPrimaryMonitor();
	auto vidmode         = glfwGetVideoMode(primary_monitor);

	int width_mm, height_mm;
	glfwGetMonitorPhysicalSize(primary_monitor, &width_mm, &height_mm);

	// As suggested by the GLFW monitor guide
	static const float inch_to_mm       = 25.0f;
	static const float win_base_density = 96.0f;

	auto dpi        = static_cast<uint32_t>(vidmode->width / (width_mm / inch_to_mm));
	auto dpi_factor = dpi / win_base_density;
	return dpi_factor;
}

float glfw_window::get_content_scale_factor() const
{
	float xscale, yscale;
	glfwGetWindowContentScale(m_handle, &xscale, &yscale);
	return xscale;
}
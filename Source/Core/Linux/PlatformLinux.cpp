#include "PreCompile.h"
#include "Pharos.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

PlatformLinux::PlatformLinux()
{
}

PlatformLinux::~PlatformLinux()
{

}

bool PlatformLinux::Init()
{
	int wndWidth = WINDOW_WIDTH;
	int wndHeight = WINDOW_HEIGHT;	
	const char8* APP_NAME = "Pharos";

	m_window = new glfw_window();
	m_window->create_window(APP_NAME, wndWidth, wndHeight);

	char8 path[MAX_PATH] = { 0 };
	readlink("/proc/self/exe", path, MAX_PATH);
	Pharos::Utils::Path path_obj(path);
	const char8* absPath = path_obj.GetFullPath();
	int32 len = (int32)strlen(absPath);
	string homePath = string(absPath) + "../";
	string bundlePath = string(absPath) + "../Data/";
	
	sKernel->SetEngineHomePath(homePath.c_str());
	sKernel->SetEngineBundlePath(bundlePath.c_str());

	if (!sKernel->Init(m_window)) return false;

	m_timer.Reset();

	return true;
}

void PlatformLinux::Destroy()
{
	SAFE_DELETE(m_window);
}

void PlatformLinux::onKeyboardEvent(const KeyEvent& keyEvent)
{
}

void PlatformLinux::onMouseEvent(int32 mouse, int action, int32 posX, int32 posY, int32 wheel)
{
	static CODE_STATE codes[2];
	codes[GLFW_PRESS] = STATE_DOWN;
	codes[GLFW_RELEASE] = STATE_UP;

	MouseEvent event;
	event.button = (mouse != -1) ? (MOUSE_CODE)mouse : MOUSE_NUM;
	event.state = (mouse == -1) ? STATE_KEEP : codes[action];
	event.x = posX;
	event.y = posY;
	event.ox = posX - m_mousePos.x;
	event.oy = posY - m_mousePos.y;
	event.wheel = wheel * 120;
	event.shift = false;
	event.ctrl = false;
	event.alt = false;

	m_mousePos.x = posX;
	m_mousePos.y = posY;

	sKernel->onMouseEvent(event);
}

void PlatformLinux::onWindowChangeSize(int32 width, int32 height)
{	
	sKernel->onWindowChangeSize(width, height);
}

int32 PlatformLinux::Run()
{
	while (!m_window->should_close())
	{
		float32 fElapsed = m_timer.GetElapsedTime();
		
		sKernel->Run(fElapsed);

		m_window->process_events();
	}

	return 0;
}
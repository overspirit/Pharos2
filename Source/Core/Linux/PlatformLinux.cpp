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

	return true;
}

void PlatformLinux::Destroy()
{
	SAFE_DELETE(m_window);
}

void PlatformLinux::onKeyboardEvent(const KeyEvent& keyEvent)
{
}

void PlatformLinux::onMouseEvent(const MouseEvent& mouseEvent)
{
}

void PlatformLinux::onWindowChangeSize(int32 width, int32 height)
{	
}

int32 PlatformLinux::Run()
{
	while (!m_window->should_close())
	{
		sKernel->Run(0);

		m_window->process_events();
	}

	return 0;
}
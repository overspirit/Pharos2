#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "DesktopGlobal.h"
#include "SceneGlobal.h"

DEFINE_int32(width, 1334, "The width of the create window.");
DEFINE_int32(height, 750, "The height of the create window.");

PlatformWindows::PlatformWindows()
{
	m_hWnd = nullptr;
}

PlatformWindows::~PlatformWindows()
{

}

bool PlatformWindows::Init()
{
	HINSTANCE hInst = GetModuleHandle(NULL);

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = &WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = _T("Pharos");
	wcex.hIconSm = nullptr;

	RegisterClassEx(&wcex);
	
	m_hWnd = CreateWindow(_T("Pharos"), _T("Demo"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInst, NULL);
	if (!m_hWnd) return false;	

	// 计算窗口客户区大小以及边框大小
	RECT rcWindow, rcClinet;
	::GetWindowRect(m_hWnd, &rcWindow);	// 计算边框的大小
	::GetClientRect(m_hWnd, &rcClinet);
	int32 borderWidth = (rcWindow.right - rcWindow.left) - (rcClinet.right - rcClinet.left);
	int32 borderHeight = (rcWindow.bottom - rcWindow.top) - (rcClinet.bottom - rcClinet.top);
	int32 wndWidth = FLAGS_width + borderWidth;
	int32 wndHeight = FLAGS_height + borderHeight;
	int32 nX = (GetSystemMetrics(SM_CXSCREEN) - wndWidth) / 2;
	int32 nY = (GetSystemMetrics(SM_CYSCREEN) - wndHeight) / 2;
	MoveWindow(m_hWnd, nX, nY, wndWidth, wndHeight, true);

	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);	

	if (!sInputMgr->InitInputDevice(m_hWnd, hInst)) return false;

	m_timer.Reset();

	char8 path[MAX_PATH] = { 0 };
	GetCurrentDirectoryA(MAX_PATH, path);
	int32 len = (int32)strlen(path);
	if (path[len] != '\\') path[len] = '\\';
	string homePath = string(path) + "../Data/";
	string bundlePath = string(path) + "../Data/";

	sKernel->SetEngineHomePath(homePath.c_str());
	sKernel->SetEngineBundlePath(bundlePath.c_str());

	if (!sKernel->Init(m_hWnd)) return false;

	return true;
}

void PlatformWindows::Destroy()
{
	sKernel->Destroy();	
	
	sInputMgr->Shutdown();

	DestroyWindow(m_hWnd);
}

LRESULT CALLBACK PlatformWindows::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SHOWWINDOW:
	{
		sPlatform->onWindowCreate();
	}
	break;
	case WM_SIZE:
	{
		sPlatform->onWindowChangeSize(LOWORD(lParam), HIWORD(lParam));
	}
	break;
	case WM_CLOSE:
	{
		PostQuitMessage(0);
	}
	break;
	case WM_DESTROY:
	{
		sPlatform->onWindowDestroy();
	}
	break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}

void PlatformWindows::onKeyboardEvent(const KeyEvent& keyEvent)
{
	if (GetActiveWindow() != m_hWnd) return;

	sKernel->onKeyboardEvent(keyEvent);
}

void PlatformWindows::onMouseEvent(const MouseEvent& mouseEvent)
{
	if (GetActiveWindow() != m_hWnd) return;
		
	sKernel->onMouseEvent(mouseEvent);
}

void PlatformWindows::onWindowCreate()
{
	sKernel->onViewCreate();
}

void PlatformWindows::onWindowChangeSize(int32 width, int32 height)
{	
	sKernel->onViewChangeSize(width, height);
}

void PlatformWindows::onWindowDestroy()
{
	sKernel->onViewDestroy();
}

int32 PlatformWindows::Run()
{
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (!sInputMgr->UpdateInputDevice())
		{
			if (!sInputMgr->ResetInputDevce())
			{
				break;
			}
		}
		
		float32 fElapsed = m_timer.GetElapsedTime();		
 
		sKernel->Run(fElapsed);
	}

	return 0;
}
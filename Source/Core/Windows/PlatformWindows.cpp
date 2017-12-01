#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
// #include "DesktopGlobal.h"
#include "SceneGlobal.h"

// DEFINE_int32(width, 1334, "The width of the create window.");
// DEFINE_int32(height, 750, "The height of the create window.");
// 
// Engine::IKernel* g_pKernel;
// Render::IRenderMgr* g_pRenderMgr;
// Scene::ISceneMgr* g_pSceneMgr;
// Desktop::IDesktopMgr* g_pDesktopMgr;

#include <tchar.h>

uint32 FLAGS_width = 1334;
uint32 FLAGS_height = 750;

ID3D11Device*			g_device;
ID3D11DeviceContext*	g_context;

vector<SceneNode*>		g_sceneNodeList;

SceneCamera				g_camera;

D3D11ConstantBuffer*	g_cb;

D3D11FrameBuffer*	g_fb;

PlatformWindows::PlatformWindows()
{
	m_hWnd = nullptr;

	m_fps = 0;
	m_renderCount = 0;
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

	//if (!sInputMgr->InitInputDevice(m_hWnd, hInst)) return false;

	//m_pTimer = MakeSharedPtr<Timer>();
	m_timer.Reset();

	char8 path[MAX_PATH] = { 0 };
	GetCurrentDirectoryA(MAX_PATH, path);
	int32 len = (int32)strlen(path);
	if (path[len] != '\\') path[len] = '\\';
	string homePath = string(path) + "../Data/";
	string bundlePath = string(path) + "../Data/";

	InitDevice();

	g_camera.BuildViewMatrix(Vector3Df(150.0f, 150.0f, -150.0f), Vector3Df(0, 0, 0));
	g_camera.BuildProjMatrix((float32)PI / 4, FLAGS_width, FLAGS_height, 1.0f, 1000.0f);

	g_cb = new D3D11ConstantBuffer(m_device, m_context);

	BundleSceneImporter importer;
	importer.LoadSceneFile("D:/HiSceneV5/HiScene-v5-4094-GS8/res/common/gs8.gpz");
	importer.FillSceneNodeList(g_sceneNodeList);

	// 	if (!sKernel->Init(m_hWnd)) return false;
	// 	sKernel->SetEngineHomePath(homePath.c_str());
	// 	sKernel->SetEngineBundlePath(bundlePath.c_str());
	// 	g_pKernel = sKernel;
	// 
	// 	if (!sRenderMgr->Init()) return false;
	// 	g_pRenderMgr = sRenderMgr;
	// 
	// 	if (!sSceneMgr->Init()) return false;
	// 	g_pSceneMgr = sSceneMgr;
	// 
	// 	if (!sDesktopMgr->Init()) return false;
	// 	g_pDesktopMgr = sDesktopMgr;
	// 
	// 	if (!sKernel->StartUp()) return false;

	return true;
}

void PlatformWindows::Destroy()
{
	// 	sDesktopMgr->Destroy();
	// 	sSceneMgr->Destroy();
	// 	sRenderMgr->Destroy();
	// 	sKernel->Destroy();	
	// 	
	// 	sInputMgr->Shutdown();

	DestroyWindow(m_hWnd);
}

bool PlatformWindows::InitDevice()
{
	unsigned int creationFlags = 0;

#ifdef _DEBUG
	//creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT result = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, creationFlags, nullptr, 0, D3D11_SDK_VERSION, &m_device, &m_featureLevel, &m_context);
	if (FAILED(result)) return false;

	IDXGIDevice* dxgiDevice = nullptr;
	result = m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	if (FAILED(result)) return false;

	result = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgiAdapter);
	if (FAILED(result)) return false;

	result = m_dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgiFactory);
	if (FAILED(result)) return false;

	// 释放COM接口
	SAFE_RELEASE(dxgiDevice);

	memset(&m_swapChainDesc, 0, sizeof(m_swapChainDesc));
	m_swapChainDesc.BufferCount = 1;
	m_swapChainDesc.BufferDesc.Width = FLAGS_width;
	m_swapChainDesc.BufferDesc.Height = FLAGS_height;
	m_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	m_swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	m_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	m_swapChainDesc.OutputWindow = m_hWnd;
	m_swapChainDesc.Windowed = true;
	m_swapChainDesc.SampleDesc.Count = 1;
	m_swapChainDesc.SampleDesc.Quality = 0;

	// 现在，创建交换链
	result = m_dxgiFactory->CreateSwapChain(m_device, &m_swapChainDesc, &m_swapChain);
	if (FAILED(result)) return false;

	g_fb = new D3D11FrameBuffer(m_device, m_context);
	g_fb->GenerateDefaultFrameBuffer(m_swapChain);
	g_fb->ApplyDevice();

	g_device = m_device;
	g_context = m_context;

	return true;
}

LRESULT CALLBACK PlatformWindows::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_SHOWWINDOW:
		{
			//sPlatform->onWindowCreate();
		}
		break;
		case WM_SIZE:
		{
			//sPlatform->onWindowChangeSize(LOWORD(lParam), HIWORD(lParam));
		}
		break;
		case WM_CLOSE:
		{
			PostQuitMessage(0);
		}
		break;
		case WM_DESTROY:
		{
			//sPlatform->onWindowDestroy();
		}
		break;
		default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}

// void PlatformWindows::onKeyboardEvent(const KeyEvent& keyEvent)
// {
// 	if (GetActiveWindow() != m_hWnd) return;
// 
// 	sDesktopMgr->onKeyboardEvent(keyEvent);
// 	sKernel->onKeyboardEvent(keyEvent);
// }
// 
// void PlatformWindows::onMouseEvent(const MouseEvent& mouseEvent)
// {
// 	if (GetActiveWindow() != m_hWnd) return;
// 
// 	sDesktopMgr->onMouseEvent(mouseEvent);
// 	sKernel->onMouseEvent(mouseEvent);
// }

void PlatformWindows::onWindowCreate()
{
	//sKernel->onViewCreate();
}

void PlatformWindows::onWindowChangeSize(int32 width, int32 height)
{
	//sDesktopMgr->onViewChangeSize(width, height);
	//sKernel->onViewChangeSize(width, height);
}

void PlatformWindows::onWindowDestroy()
{
	//sKernel->onViewDestroy();
}

int32 PlatformWindows::Run()
{
	struct ConstantBuffer
	{
		Matrix4		world;
		Matrix4		view;
		Matrix4		proj;
	};

	ConstantBuffer	cb;	

	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		g_fb->ClearFrameBuffer(0xFF7F7F7F);

		cb.world;
		cb.view = g_camera.GetViewMatrix();
		cb.proj = g_camera.GetProjMatrix();

		g_cb->CopyData(&cb, sizeof(ConstantBuffer));

		g_cb->ApplyToDevice(0);
		
		for (uint32 i = 0; i < g_sceneNodeList.size(); i++)
		{
			g_sceneNodeList[i]->Update(0);
		}

		m_swapChain->Present(0, 0);

		m_renderCount++;
		if (m_timer.GetTime() >= 1.0f)
		{
			m_timer.Reset();
			m_fps = m_renderCount;
			m_renderCount = 0;

			char8 buf[32];
			sprintf(buf, "FPS:%d\n", m_fps);
			OutputDebugStringA(buf);
		}

		// 		if (!sInputMgr->UpdateInputDevice())
		// 		{
		// 			if (!sInputMgr->ResetInputDevce())
		// 			{
		// 				throw EXCEPTION("DirectInput Error!");
		// 			}
		// 		}
		// 
		// 		float32 fElapsed = m_pTimer->GetElapsedTime();
		// 		
		// 		sSceneMgr->Update(fElapsed);
		// 		sDesktopMgr->Update(fElapsed);
		// 		sRenderMgr->Update(fElapsed);
		// 		sKernel->Update(fElapsed);
		// 
		// 		
		// 		sSceneMgr->Render(fElapsed);
		// 		sDesktopMgr->Render(fElapsed);
		// 		sRenderMgr->Render(fElapsed);
		// 		sKernel->Render(fElapsed);
	}

	return 0;
}
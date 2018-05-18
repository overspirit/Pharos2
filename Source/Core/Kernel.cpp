#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "DesktopGlobal.h"
#include "SceneGlobal.h"

Kernel::Kernel()
{
	m_hWnd = nullptr;

	m_fontLib = nullptr;

	m_pApp = nullptr;
}

Kernel::~Kernel()
{
	FreeImage_DeInitialise();

	FT_Done_FreeType(m_fontLib);
}

bool Kernel::Init(const void* hWnd)
{
	m_hWnd = hWnd;

	FT_Init_FreeType(&m_fontLib);
	FreeImage_Initialise();
    
#pragma message("------------------------------------资源的异步加载未完成!!!------------------------------------")
#pragma message("------------------------------------资源的引用计数未完成!!!------------------------------------")

	if (!sRenderMgr->Init()) return false;
	if (!sSceneMgr->Init()) return false;
	if (!sDesktopMgr->Init()) return false;
	if (!sKernel->StartUp()) return false;

	return true;
}

bool Kernel::StartUp()
{
	m_pApp = NewApplication();

	if (!m_pApp->Init()) return false;

	return true;
}

void Kernel::Destroy()
{
	//由于Kernel的析构函数会进行一些内存回收工作
	//所以要求Kernel持有的所有对象必须在Destroy方法内回收...
	m_pApp->Destroy();
	SAFE_DELETE(m_pApp);	//在这里析构IApplication对象，因为这个对象可能持有其他对象...	

	sDesktopMgr->Destroy();
	sSceneMgr->Destroy();
	sRenderMgr->Destroy();
}

void Kernel::onKeyboardEvent(const KeyEvent& keyEvent)
{
	sDesktopMgr->onKeyboardEvent(keyEvent);
	if (m_pApp != nullptr) m_pApp->onKeyboardEvent(keyEvent);
}

void Kernel::onMouseEvent(const MouseEvent& mouseEvent)
{
	sDesktopMgr->onMouseEvent(mouseEvent);
	if (m_pApp != nullptr) m_pApp->onMouseEvent(mouseEvent);
}

void Kernel::onViewCreate()
{
	if (m_pApp != nullptr) m_pApp->onViewCreate();
}

void Kernel::onViewChangeSize(int32 width, int32 height)
{
	m_wndSize.width = width;
	m_wndSize.height = height;
	
	sDesktopMgr->onViewChangeSize(width, height);

	if (m_pApp != nullptr) m_pApp->onViewChangeSize(width, height);
}

void Kernel::onViewDestroy()
{
	if (m_pApp != nullptr) m_pApp->onViewDestroy();
}

void Kernel::Run(float32 fElapsed)
{
	sSceneMgr->Update(fElapsed);
	sDesktopMgr->Update(fElapsed);
	sRenderMgr->Update(fElapsed);

	if (m_pApp != nullptr) m_pApp->Update(fElapsed);

	sSceneMgr->Render(fElapsed);
	sDesktopMgr->Render(fElapsed);
	sRenderMgr->Render(fElapsed);

	if (m_pApp != nullptr) m_pApp->Render(fElapsed);
}

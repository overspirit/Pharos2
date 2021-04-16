#include "PreCompile.h"
#include "Pharos.h"

Kernel::Kernel()
{
	m_hWnd = nullptr;
	
	m_pApp = nullptr;

    m_runFlag = -1;
}

Kernel::~Kernel()
{

}

bool Kernel::Init(const void* hWnd, int32 wndWidth, int32 wndHeight)
{
	m_hWnd = hWnd;
	m_wndSize.width = wndWidth;
	m_wndSize.height = wndHeight;

    if (!sResMgr->Init()) return false;

    if (!sRenderMgr->Init()) return false;
    if (!sSceneMgr->Init()) return false;
    if (!sDesktopMgr->Init()) return false;
	
    m_pApp = NewApplication();
    if (!m_pApp->Init()) return false;
    
	return true;
}

bool Kernel::StartUp()
{
    //打开窗口之类的...
    //...
    
    m_runFlag = 0;

	return true;
}

void Kernel::ShutingDown()
{
    m_runFlag = -1;
}

void Kernel::Restart()
{
    m_runFlag = 1;
}

void Kernel::Destroy()
{
	//由于Kernel的析构函数会进行一些内存回收工作
	//所以要求Kernel持有的所有对象必须在Destroy方法内回收...
	if(m_pApp != NULL) m_pApp->Destroy();
	SAFE_DELETE(m_pApp);	//在这里析构IApplication对象，因为这个对象可能持有其他对象...	

    sDesktopMgr->Destroy();
    sSceneMgr->Destroy();
    sRenderMgr->Destroy();

    sResMgr->Destroy();
}

void Kernel::onKeyboardEvent(const KeyEvent& keyEvent)
{	
    if (!sDesktopMgr->onKeyboardEvent(keyEvent) && m_pApp != nullptr)
    {
        m_pApp->onKeyboardEvent(keyEvent);
    }
}

void Kernel::onMouseEvent(const MouseEvent& mouseEvent)
{	
    if (!sDesktopMgr->onMouseEvent(mouseEvent) && m_pApp != nullptr)
    {
        m_pApp->onMouseEvent(mouseEvent);
    }
}

void Kernel::onWindowChangeSize(int32 width, int32 height)
{
    m_wndSize.width = width;
    m_wndSize.height = height;

    sDesktopMgr->onViewChangeSize(width, height);

    if (m_pApp != nullptr) m_pApp->onWindowChangeSize(width, height);
}

int32 Kernel::Run(float32 fElapsed)
{
    if (m_pApp != nullptr) m_pApp->onPreSceneUpdate(fElapsed);

    sSceneMgr->Update(fElapsed);

    if (m_pApp != nullptr) m_pApp->onPostSceneUpdate(fElapsed);

    sDesktopMgr->Update(fElapsed);

    if (m_pApp != nullptr) m_pApp->onPreRender(fElapsed);
    
    sRenderMgr->Render(fElapsed);

    if (m_pApp != nullptr) m_pApp->onPostRender(fElapsed);
    
    return m_runFlag;
}

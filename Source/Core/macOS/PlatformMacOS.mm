#include "PreCompile.h"
#include "Pharos.h"

PlatformMacOS::PlatformMacOS()
{
    
}

PlatformMacOS::~PlatformMacOS()
{

}

bool PlatformMacOS::Init()
{
    NSApplication* applicaton = [NSApplication sharedApplication];
    id delegate = applicaton.delegate;
    
    m_view = [[MTKView alloc] init];

    
    NSWindowStyleMask style = NSWindowStyleMaskTitled | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable | NSWindowStyleMaskClosable;
    m_window = [[NSWindow alloc] initWithContentRect: CGRectMake(0, 0, 800, 600) styleMask:style backing: NSBackingStoreBuffered defer: NO];
    
    [m_window setContentView: m_view];
    [m_window setDelegate: delegate];
    [m_window makeFirstResponder: delegate];
    [m_window center];
    [m_window makeKeyWindow];
    [m_window setMinSize:CGSizeMake(400,300)];
    
    [NSApp beginModalSessionForWindow: m_window];    
    
    
    m_view.device = MTLCreateSystemDefaultDevice();
    
    //Renderer* renderer = [[Renderer alloc] initWithMetalKitView: view];
    //[renderer mtkView: view drawableSizeWillChange: view.bounds.size];
    m_view.delegate = delegate;
    
    m_timer.Reset();

//    char8 path[MAX_PATH] = { 0 };
//    GetCurrentDirectoryA(MAX_PATH, path);
//    int32 len = (int32)strlen(path);
//    if (path[len] != '\\') path[len] = '\\';
//    string homePath = string(path) + "../";
//    string bundlePath = string(path) + "../Data/";
//
//    sKernel->SetEngineHomePath(homePath.c_str());
//    sKernel->SetEngineBundlePath(bundlePath.c_str());
//
    if (!sKernel->Init((__bridge void*)m_view)) return false;

	return true;
}

void PlatformMacOS::Destroy()
{
	sKernel->Destroy();
}

//void PlatformMacOS::onKeyboardEvent(const Pharos::KeyEvent& keyEvent)
//{
	//if (GetActiveWindow() != m_hWnd) return;

	//sKernel->onKeyboardEvent(keyEvent);
//}

//void PlatformMacOS::onMouseEvent(const Pharos::MouseEvent& mouseEvent)
//{
	//if (GetActiveWindow() != m_hWnd) return;
		
	//sKernel->onMouseEvent(mouseEvent);
//}

void PlatformMacOS::onWindowCreate()
{
	sKernel->onViewCreate();
}

void PlatformMacOS::onWindowChangeSize(int32 width, int32 height)
{	
	sKernel->onViewChangeSize(width, height);
}

void PlatformMacOS::onWindowDestroy()
{
	sKernel->onViewDestroy();
}

void PlatformMacOS::Update()
{
    float32 fElapsed = m_timer.GetElapsedTime();
    sKernel->Run(fElapsed);
}

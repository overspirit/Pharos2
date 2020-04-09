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
    
    int32 wnd_width = 1280;
    int32 wnd_height = 720;
    
    NSRect wnd_rect = CGRectMake(0, 0, wnd_width, wnd_height);
    
    m_view = [[MTKView alloc] init];
    [m_view setDelegate: delegate];
    
    NSWindowStyleMask style = NSWindowStyleMaskTitled | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable | NSWindowStyleMaskClosable;
    m_window = [[NSWindow alloc] initWithContentRect: wnd_rect styleMask:style backing: NSBackingStoreBuffered defer: NO];
    [m_window setContentView: m_view];
    [m_window center];
    [m_window makeKeyWindow];
    
    m_trackingArea = [[NSTrackingArea alloc] initWithRect:wnd_rect options: NSTrackingMouseMoved | NSTrackingActiveInKeyWindow owner:delegate userInfo:nil];
    [m_view addTrackingArea: m_trackingArea];
    
    NSWindowController* windowController = delegate;
    windowController.window = m_window;
    
    [NSApp beginModalSessionForWindow: m_window];
    
    m_timer.Reset();
    
    NSString* appPath = [[NSBundle mainBundle] bundlePath];
    string homePath = [appPath UTF8String];
    string bundlePath = homePath + "/Data/";

    sKernel->SetEngineHomePath(homePath.c_str());
    sKernel->SetEngineBundlePath(bundlePath.c_str());

    
    
    m_view.device = MTLCreateSystemDefaultDevice();
    //Renderer* renderer = [[Renderer alloc] initWithMetalKitView: view];
    //[renderer mtkView: view drawableSizeWillChange: view.bounds.size];
    //m_view.delegate = delegate;
    
    if (!sKernel->Init((__bridge void*)m_view)) return false;
    

    
	return true;
}

void PlatformMacOS::Destroy()
{
	sKernel->Destroy();
}

void PlatformMacOS::onKeyboardEvent(NSEvent* keyEvent)
{
    NSLog(@"key event type: %lu, char: %@", keyEvent.type, keyEvent.characters);
}

void PlatformMacOS::onMouseEvent(NSEvent* mouseEvent)
{
    NSPoint pos = mouseEvent.locationInWindow;
    NSLog(@"mouse event type: %lu pos:{x:%f, y:%f}", mouseEvent.type, pos.x, pos.y);
}

void PlatformMacOS::onViewChangeSize(int32 width, int32 height)
{	
	sKernel->onWindowChangeSize(width, height);
}

void PlatformMacOS::Update()
{
    float32 fElapsed = m_timer.GetElapsedTime();
    sKernel->Run(fElapsed);
}

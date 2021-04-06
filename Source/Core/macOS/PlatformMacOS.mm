#include "PreCompile.h"
#include "Pharos.h"

PlatformMacOS::PlatformMacOS()
{
    m_mouseState = STATE_KEEP;
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
	[m_view setDelegate : delegate];

	NSWindowStyleMask style = NSWindowStyleMaskTitled | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable | NSWindowStyleMaskClosable;
	m_window = [[NSWindow alloc] initWithContentRect:wnd_rect styleMask : style backing : NSBackingStoreBuffered defer : NO];
	[m_window setContentView : m_view];
	[m_window center];
	[m_window makeKeyWindow];

	[NSApp beginModalSessionForWindow : m_window];

	m_timer.Reset();

	NSString* appPath = [[NSBundle mainBundle] bundlePath];
	string homePath = [appPath UTF8String] + string("/");
	string bundlePath = homePath + "Contents/Resources/";

	sKernel->SetEngineHomePath(homePath.c_str());
	sKernel->SetEngineBundlePath(bundlePath.c_str());

    if (!sKernel->Init((__bridge void*)m_view, wnd_width, wnd_height)) return false;
    //if (!sKernel->Init((__bridge void*)m_view, m_viewSize.width, m_viewSize.height)) return false;

	return true;
}

void PlatformMacOS::Destroy()
{
	sKernel->Destroy();
}

void PlatformMacOS::onKeyboardEvent(NSEvent* keyEvent)
{
    if (!sKernel->isRunning()) return;
    
	//NSLog(@"key event type: %lu, char: %@", keyEvent.type, keyEvent.characters);        
        
    KeyEvent myEvent;
    myEvent.key = (KEY_CODE)keyEvent.keyCode;//([keyEvent.characters UTF8String][0]);
    myEvent.state = (CODE_STATE)((keyEvent.type - NSEventTypeKeyDown) + STATE_DOWN);
    myEvent.shift = false;
    myEvent.ctrl = false;
    myEvent.alt = false;
    
    sKernel->onKeyboardEvent(myEvent);
}

void PlatformMacOS::onMouseEvent(NSEvent* mouseEvent)
{
    if (!sKernel->isRunning()) return;
    
    NSPoint mousePos = [m_window convertPointFromScreen : NSEvent.mouseLocation];
	uint32 x = mousePos.x;
	uint32 y = m_view.drawableSize.height - mousePos.y;

    
	MouseEvent myEvent;
	myEvent.button = MOUSE_NUM;
	myEvent.state = STATE_KEEP;
	myEvent.x = x;
	myEvent.y = y;
	myEvent.ox = x - m_mousePos.x;
	myEvent.oy = y - m_mousePos.y;
    myEvent.wheel = 0;
	myEvent.shift = false;
	myEvent.ctrl = false;
	myEvent.alt = false;

	switch (mouseEvent.type)
	{
	case NSEventTypeLeftMouseDown: myEvent.button = MOUSE_LEFT; myEvent.state = STATE_DOWN; break;
	case NSEventTypeLeftMouseUp: myEvent.button = MOUSE_LEFT; myEvent.state = STATE_UP; break;
	case NSEventTypeRightMouseDown: myEvent.button = MOUSE_RIGHT; myEvent.state = STATE_DOWN; break;
	case NSEventTypeRightMouseUp: myEvent.button = MOUSE_RIGHT; myEvent.state = STATE_UP; break;
	case NSEventTypeOtherMouseDown: myEvent.button = MOUSE_MID; myEvent.state = STATE_DOWN; break;
	case NSEventTypeOtherMouseUp: myEvent.button = MOUSE_MID; myEvent.state = STATE_UP; break;
	case NSEventTypeLeftMouseDragged: myEvent.button = MOUSE_LEFT; myEvent.state = STATE_DOWN; break;
	case NSEventTypeRightMouseDragged: myEvent.button = MOUSE_RIGHT; myEvent.state = STATE_DOWN; break;
	case NSEventTypeScrollWheel: myEvent.wheel = (uint32)mouseEvent.scrollingDeltaY * 120; break;
	case NSEventTypeOtherMouseDragged: myEvent.button = MOUSE_MID; myEvent.state = STATE_DOWN; break;
    case NSEventTypeMouseMoved: break;
        default: return;
	}
    
    if (m_mouseState == myEvent.state)
    {
        myEvent.state = STATE_KEEP;
    }
    else
    {
        m_mouseState = myEvent.state;
    }
    
	//NSLog(@"mouse event button:%d state:%d ox:%d oy:%d", myEvent.button,  myEvent.state, myEvent.ox, myEvent.oy);

    sKernel->onMouseEvent(myEvent);

	m_mousePos.x = x;
	m_mousePos.y = y;
}

void PlatformMacOS::onViewChangeSize(int32 width, int32 height)
{
    m_viewSize.width = width;
    m_viewSize.height = height;
    
    if (!sKernel->isRunning()) return;
    
    sKernel->onWindowChangeSize(width, height);
}

void PlatformMacOS::Update()
{
	float32 fElapsed = m_timer.GetElapsedTime();

	//NSLog(@"elapsed:%f", fElapsed);    

    if (!sKernel->isRunning()) return;
    
    sKernel->Run(fElapsed);
}

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
	[m_view setDelegate : delegate];

	NSWindowStyleMask style = NSWindowStyleMaskTitled | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable | NSWindowStyleMaskClosable;
	m_window = [[NSWindow alloc] initWithContentRect:wnd_rect styleMask : style backing : NSBackingStoreBuffered defer : NO];
	[m_window setContentView : m_view];
	[m_window center];
	[m_window makeKeyWindow];

	//    m_trackingArea = [[NSTrackingArea alloc] initWithRect:wnd_rect options: NSTrackingMouseMoved | NSTrackingActiveInKeyWindow owner:delegate userInfo:nil];
	//    [m_view addTrackingArea: m_trackingArea];

	//    NSWindowController* windowController = delegate;
	//    windowController.window = m_window;

	[NSApp beginModalSessionForWindow : m_window];

	m_timer.Reset();

	NSString* appPath = [[NSBundle mainBundle] bundlePath];
	string homePath = [appPath UTF8String] + string("/");
	string bundlePath = homePath + "Contents/Resources/";

	sKernel->SetEngineHomePath(homePath.c_str());
	sKernel->SetEngineBundlePath(bundlePath.c_str());

	if (!sKernel->Init((__bridge void*)m_view, wnd_width, wnd_height)) return false;

	return true;
}

void PlatformMacOS::Destroy()
{
	sKernel->Destroy();
}

void PlatformMacOS::onKeyboardEvent(NSEvent* keyEvent)
{
	//NSLog(@"key event type: %lu, char: %@", keyEvent.type, keyEvent.characters);
}

void PlatformMacOS::onMouseEvent(NSEvent* mouseEvent)
{
	uint32 x = mouseEvent.locationInWindow.x;
	uint32 y = m_view.drawableSize.height - mouseEvent.locationInWindow.y;

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
	case NSEventTypeScrollWheel: break;
	case NSEventTypeOtherMouseDragged: myEvent.button = MOUSE_MID; myEvent.state = STATE_DOWN; break;
	}

	//NSLog(@"mouse event button:%d state:%d ox:%d oy:%d", myEvent.button,  myEvent.state, myEvent.ox, myEvent.oy);

	sKernel->onMouseEvent(myEvent);

	m_mousePos.x = x;
	m_mousePos.y = y;
}

void PlatformMacOS::onViewChangeSize(int32 width, int32 height)
{
	sKernel->onWindowChangeSize(width, height);
}

void PlatformMacOS::Update()
{
	float32 fElapsed = m_timer.GetElapsedTime();

	//NSLog(@"elapsed:%f", fElapsed);

	sKernel->Run(fElapsed);
}

//
//  main.m
//  ModelViewer
//
//  Created by v_xiaowenzhang on 2020/4/2.
//  Copyright © 2020 mindbrain. All rights reserved.
//
#include "PreCompile.h"
#include "Pharos.h"

@interface AppDelegate : NSObject <NSApplicationDelegate, MTKViewDelegate>

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	sPlatform->Init();
}

-(void)applicationWillTerminate : (NSNotification *)aNotification
{
	sPlatform->Destroy();
}

-(BOOL)applicationShouldTerminateAfterLastWindowClosed : (NSApplication *)sender
{
	return YES;
}

-(void)keyEvent : (NSEvent *)event
{
	sPlatform->onKeyboardEvent(event);
}

-(void)mouseEvent : (NSEvent *)event
{
	sPlatform->onMouseEvent(event);
}

-(void)mtkView : (nonnull MTKView *)view drawableSizeWillChange : (CGSize)size
{
	sPlatform->onViewChangeSize(size.width, size.height);
}

-(void)drawInMTKView : (nonnull MTKView *)view
{
	sPlatform->Update();
}

@end

id monitorId;

void addMonitor()
{
    NSEventMask mask = NSEventMaskLeftMouseDown | NSEventMaskLeftMouseUp |
        NSEventMaskRightMouseDown | NSEventMaskRightMouseUp |
        NSEventMaskOtherMouseDown | NSEventMaskOtherMouseUp |
        NSEventMaskScrollWheel | NSEventMaskMouseMoved |
        NSEventMaskLeftMouseDragged | NSEventMaskRightMouseDragged | NSEventMaskOtherMouseDragged |
        NSEventMaskKeyDown | NSEventMaskKeyUp;

    monitorId = [NSEvent addLocalMonitorForEventsMatchingMask : mask handler : ^NSEvent*_Nullable(NSEvent *event) {
        //NSLog(@"local monitor: %@", event);

        NSApplication* applicaton = [NSApplication sharedApplication];
        AppDelegate* delegate = (AppDelegate*)applicaton.delegate;

        switch (event.type)
        {
            case NSEventTypeKeyUp:
            case NSEventTypeKeyDown:
                [delegate keyEvent:event];
                break;
            case NSEventTypeLeftMouseDown:
            case NSEventTypeLeftMouseUp:
            case NSEventTypeRightMouseDown:
            case NSEventTypeRightMouseUp:
            case NSEventTypeOtherMouseDown:
            case NSEventTypeOtherMouseUp:
            case NSEventTypeMouseMoved:
            case NSEventTypeScrollWheel:
            case NSEventTypeLeftMouseDragged:
            case NSEventTypeRightMouseDragged:
            case NSEventTypeOtherMouseDragged:
                [delegate mouseEvent:event];
                break;
        }

        return event;
    }];
}

const char* OpenFileDialog(const char* defaultPath)
{
    [NSEvent removeMonitor: monitorId];
    
    NSString* dirPath = NSHomeDirectory();
    if (defaultPath != NULL)
    {
        dirPath = [NSString stringWithUTF8String:defaultPath];
    }
    
    
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    [panel setDirectoryURL: [NSURL URLWithString: dirPath]];
    [panel setAllowsMultipleSelection:NO];
    [panel setCanChooseDirectories:NO];
    [panel setCanChooseFiles:YES];
    [panel setAllowedFileTypes:[NSArray arrayWithObjects:@"png",@"jpg",@"fbx",@"sceneml", nil]];
    [panel setAllowsOtherFileTypes:YES];
    if ([panel runModal] == NSModalResponseOK) {
        addMonitor();
        return [[panel.URLs.firstObject path] UTF8String];
    }
    
    addMonitor();
    return "";
}

int main(int argc, const char * argv[])
{
	NSApplication* applicaton = [NSApplication sharedApplication];
	AppDelegate* delegate = [[AppDelegate alloc] init];
	applicaton.delegate = delegate;

	NSEventMask mask = NSEventMaskLeftMouseDown | NSEventMaskLeftMouseUp |
		NSEventMaskRightMouseDown | NSEventMaskRightMouseUp |
		NSEventMaskOtherMouseDown | NSEventMaskOtherMouseUp |
		NSEventMaskScrollWheel | NSEventMaskMouseMoved |
		NSEventMaskLeftMouseDragged | NSEventMaskRightMouseDragged | NSEventMaskOtherMouseDragged |
		NSEventMaskKeyDown | NSEventMaskKeyUp;

    addMonitor();

	return NSApplicationMain(argc, argv);
}


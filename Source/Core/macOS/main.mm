//
//  main.m
//  ModelViewer
//
//  Created by v_xiaowenzhang on 2020/4/2.
//  Copyright © 2020 mindbrain. All rights reserved.
//
#include "PreCompile.h"
#include "Pharos.h"

@interface AppDelegate : NSWindowController <NSApplicationDelegate, MTKViewDelegate>

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    sPlatform->Init();
}

- (void)applicationWillTerminate:(NSNotification *)aNotification
{
    sPlatform->Destroy();
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
    return YES;
}

- (void)keyUp:(NSEvent *)event
{
    sPlatform->onKeyboardEvent(event);
}

- (void)mouseUp:(NSEvent *)event
{
    sPlatform->onMouseEvent(event);
}

- (void)rightMouseUp:(NSEvent *)event
{
    sPlatform->onMouseEvent(event);
}

- (void)otherMouseUp:(NSEvent *)event
{
    sPlatform->onMouseEvent(event);
}

- (void)mouseMoved:(NSEvent *)event
{
    sPlatform->onMouseEvent(event);
}

- (void)scrollWheel:(NSEvent *)event
{
    sPlatform->onMouseEvent(event);
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size
{
    sPlatform->onViewChangeSize(size.width, size.height);
}

- (void)drawInMTKView:(nonnull MTKView *)view
{
    sPlatform->Update();
}

@end

int main(int argc, const char * argv[])
{
    NSApplication* applicaton = [NSApplication sharedApplication];
    AppDelegate* delegate = [[AppDelegate alloc] init];
    applicaton.delegate = delegate;
    
    return NSApplicationMain(argc, argv);
}


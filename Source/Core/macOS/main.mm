//
//  main.m
//  ModelViewer
//
//  Created by v_xiaowenzhang on 2020/4/2.
//  Copyright © 2020 mindbrain. All rights reserved.
//
#include "PreCompile.h"
#include "Pharos.h"

@interface AppDelegate : NSResponder <NSApplicationDelegate, NSWindowDelegate, MTKViewDelegate>

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
    
}

- (void)windowDidResize:(NSNotification *)notification
{
    
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size
{
    
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


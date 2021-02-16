#pragma once

//standard
//////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <memory>
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <deque>
#include <queue>
#include <set>
#include <thread>

using namespace std;
//////////////////////////////////////////////////////////////////////////

#include "CompileConfig.h"



//Windows
//////////////////////////////////////////////////////////////////////////
#if defined(_WINDOWS_PLATFORM_)

#pragma warning(disable:4996)

#include <tchar.h>
#include <io.h>
#include <assert.h>

#include <Windows.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#define DIRECTINPUT_VERSION		0x0800
#include <dinput.h>
#include <XInput.h>
#include <oleauto.h>
#include <wbemidl.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "XInput.lib")

#endif
//////////////////////////////////////////////////////////////////////////



//macOS
//////////////////////////////////////////////////////////////////////////
#if defined(_MACOS_PLATFORM_)

#pragma clang diagnostic ignored "-Wdocumentation"
#pragma clang diagnostic ignored "-Wswitch"

#define MAX_PATH 256
#define ZeroMemory(p, l) memset((p), 0, (l))

#ifdef __OBJC__

#import <Cocoa/Cocoa.h>
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>

#endif

#include <netinet/in.h>
#include <mach/mach_time.h>
#include <unistd.h>

#endif
//////////////////////////////////////////////////////////////////////////



//iOS
//////////////////////////////////////////////////////////////////////////
#if defined(_IOS_PLATFORM_)

#define MAX_PATH 256
#define ZeroMemory(p, l) memset((p), 0, (l))

//#import <OpenGLES/ES1/gl.h>
//#import <OpenGLES/ES1/glext.h>
//#import <OpenGLES/ES2/gl.h>
//#import <OpenGLES/ES2/glext.h>

#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#include <mach/mach_time.h>

#endif
//////////////////////////////////////////////////////////////////////////



//Android
//////////////////////////////////////////////////////////////////////////
#if defined(_ANDROID_PLATFORM_)

#define MAX_PATH 256
#define ZeroMemory(p, l) memset((p), 0, (l))

#include <jni.h>
#include <errno.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/resource.h>
#include <linux/in.h>
#include <time.h>

#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <cassert>

#include <poll.h>
#include <pthread.h>
#include <sched.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_android.h>

#endif
////////////////////////////////////////////////////////////////////////

#if defined(_LINUX_PLATFORM_)

#define MAX_PATH 256
#define ZeroMemory(p, l) memset((p), 0, (l))

#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <cassert>

#include <xcb/xcb.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_xcb.h>

#endif

//3rd party
////////////////////////////////////////////////////////////////////////
#define FT2_BUILD_LIBRARY
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_INTERNAL_OBJECTS_H

#define FREEIMAGE_LIB
#include <FreeImage.h>

//  #define USE_DL_PREFIX
//  #define DLMALLOC_EXPORT
//  #include <dlmalloc.h>
//  
#include <rapidxml.hpp>
using namespace rapidxml;

#include <document.h>
#include <prettywriter.h>
#include <stringbuffer.h>

#include <unzip.h>
#include <zip.h>

//仅在windows下使用gflags库解析命令参数
////////////////////////////////////////////////////////////////////////
#if defined(_WINDOWS_PLATFORM_)

#include <gflags.h>

#endif
////////////////////////////////////////////////////////////////////////

//在linux下使用glfw库建立窗口
////////////////////////////////////////////////////////////////////////
#if defined(_LINUX_PLATFORM_)

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

// #include <SPIRV/GLSL.std.450.h>
// #include <SPIRV/GlslangToSpv.h>
// #include <StandAlone/ResourceLimits.h>
// #include <glslang/OSDependent/osinclude.h>
// #include <glslang/Include/ShHandle.h>
// #include <glslang/Include/revision.h>


#endif
////////////////////////////////////////////////////////////////////////

#define new DEBUG_NEW

////////////////////////////////////////////////////////////////////////




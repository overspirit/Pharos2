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
#include <set>

using namespace std;
//////////////////////////////////////////////////////////////////////////

#include "CompileConfig.h"

//System
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

#if defined(_ANDROID_PLATFORM_)

#define MAX_PATH 256
#define ZeroMemory(p, l) memset((p), 0, (l))

#include <jni.h>
#include <errno.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/resource.h>

#include <poll.h>
#include <pthread.h>
#include <sched.h>

#include <EGL/egl.h>
#include <GLES3/gl3.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#endif
////////////////////////////////////////////////////////////////////////

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


//����windows��ʹ��gflags������������
////////////////////////////////////////////////////////////////////////
#if defined(_WINDOWS_PLATFORM_)

#include <gflags.h>

#endif
////////////////////////////////////////////////////////////////////////

#define new DEBUG_NEW

////////////////////////////////////////////////////////////////////////
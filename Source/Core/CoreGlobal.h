#pragma once

#include "EventDefine.h"

//Windows
//////////////////////////////////////////////////////////////////////////
#if defined(_WINDOWS_PLATFORM_)

#include "Windows/InputSystem/InputDevice.h"
#include "Windows/InputSystem/KeyBoardDevice.h"
#include "Windows/InputSystem/MouseDevice.h"
#include "Windows/InputSystem/SofeMouseDevice.h"
#include "Windows/InputSystem/GamepadDevice.h"
#include "Windows/InputSystem/XInputDevice.h"
#include "Windows/InputSystem/InputManager.h"

#include "Windows/Timer.h"
#include "Windows/PlatformWindows.h"

#define sInputMgr CInputManager::Inst()
#define sPlatform PlatformWindows::Inst()

#endif
//////////////////////////////////////////////////////////////////////////



//macOS
//////////////////////////////////////////////////////////////////////////
#if defined(_MACOS_PLATFORM_)

#include "macOS/Timer.h"

#ifdef __OBJC__

#include "macOS/PlatformMacOS.h"
#define sPlatform PlatformMacOS::Inst()

#endif

#endif
//////////////////////////////////////////////////////////////////////////



//iOS
//////////////////////////////////////////////////////////////////////////
#if defined(_IOS_PLATFORM_)

#include "iOS/Timer.h"
#include "iOS/PlatformIOS.h"

const char* GetDocumentDirectory();
const char* GetMainBundleDirectory();

#endif
//////////////////////////////////////////////////////////////////////////




//Android
//////////////////////////////////////////////////////////////////////////
#if defined(_ANDROID_PLATFORM_)

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "Android Application", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "Android Application", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "Android Application", __VA_ARGS__))

#ifndef NDEBUG
#  define LOGV(...)  ((void)__android_log_print(ANDROID_LOG_VERBOSE, "Android Application", __VA_ARGS__))
#else
#  define LOGV(...)  ((void)0)
#endif

#include "Android/Timer.h"
#include "Android/AndroidWindow.h"
#include "Android/PlatformAndroid.h"

#define sPlatform PlatformAndroid::Inst()

#define MyWindow AndroidWindow

#endif
//////////////////////////////////////////////////////////////////////////


//Linux
//////////////////////////////////////////////////////////////////////////
#if defined(_LINUX_PLATFORM_)

#include "Linux/Timer.h"
#include "Linux/glfw_window.h"
#include "Linux/PlatformLinux.h"

#define sPlatform PlatformLinux::Inst()

#define MyWindow glfw_window:

#endif
//////////////////////////////////////////////////////////////////////////


#include "File.h"

#include "MemoryBuffer.h"

#include "ResBase.h"
#include "XmlAttribute.h"
#include "XmlNode.h"
#include "XmlDocument.h"
#include "Font.h"
#include "Image.h"
#include "Properties.h"
#include "ResourceManager.h"

#include "IApplication.h"
#include "Kernel.h"

#define sKernel Kernel::Inst()
#define sResMgr ResourceManager::Inst()



using namespace Pharos;
using namespace Pharos::Core;
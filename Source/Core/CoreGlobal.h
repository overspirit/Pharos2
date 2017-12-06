#pragma once

#include "Pharos.h"

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


#if defined(_IOS_PLATFORM_)

#include "iOS/Timer.h"
#include "iOS/PlatformIOS.h"

const char* GetDocumentDirectory();
const char* GetMainBundleDirectory();

#endif

#if defined(_ANDROID_PLATFORM_)

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "Android Application", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "Android Application", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "Android Application", __VA_ARGS__))

/*对于调试版本，始终在此库中启用调试跟踪*/
#ifndef NDEBUG
#  define LOGV(...)  ((void)__android_log_print(ANDROID_LOG_VERBOSE, "Android Application", __VA_ARGS__))
#else
#  define LOGV(...)  ((void)0)
#endif

#include "Android/TimerAndroid.h"
#include "Android/FileAndroid.h"
#include "Android/AppDelegate.h"
//#include "Android/EngineAndroid.h"
#include "Android/AndroidGlue.h"

//#define sKernel EngineAndroid::Inst()
#define sGLUE AndroidGlue::Inst()

#endif

#include "File.h"
#include "MemoryBuffer.h"

#include "ResBase.h"
#include "XmlAttribute.h"
#include "XmlNode.h"
#include "XmlDocument.h"
#include "Font.h"
#include "Image.h"
//#include "Properties.h"

#include "Kernel.h"

#define sKernel Kernel::Inst()

using namespace Core;

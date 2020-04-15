#pragma once

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
#define _WINDOWS_PLATFORM_
#endif

#if defined(_MSC_VER) && (_MSC_VER < 1400)
#  error "Only Microsoft Visual Studio 7.0 and later are supported."
#endif

#if defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
#define _IOS_PLATFORM_
#endif

#if defined (__MAC_OS_X_VERSION_MIN_REQUIRED)
#define _MACOS_PLATFORM_
#endif

////#if (__IPHONE_OS_VERSION_MIN_REQUIRED <= __IPHONE_7_0)
////#  error "Only IOS 7.1 and later are supported."
//#endif

#if defined(__ANDROID__)
#define _ANDROID_PLATFORM_
#endif


#if !defined(REALINLINE)
#if defined(_MSC_VER)
#define REALINLINE __forceinline
#else
#define REALINLINE inline
#endif
#endif

//----------------------------
#if !defined(SAFE_DELETE)
#define SAFE_DELETE(p) if((p)!=NULL) {delete (p);(p)=NULL;}
#endif

#if !defined(SAFE_RELEASE)
#define SAFE_RELEASE(p) if((p)!=NULL)	{(p)->Release();(p)=NULL;}
#endif

#if !defined(SAFE_FREE)
#define SAFE_FREE(p) if((p)!=NULL) {free(p);(p)=NULL;}
#endif

#if !defined(SAFE_DELETEA)
#define SAFE_DELETEA(p) if((p)!=NULL) {delete[] (p);(p)=NULL;}
#endif

#define TYPE_NAME(t) (#t)
//----------------------------

//////////////////////////////////////////////////////////////////////////
#if defined(_DEBUG)

#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)

#define TRACE printf

#else

#define DEBUG_NEW new

#define TRACE

#endif
//////////////////////////////////////////////////////////////////////////

#pragma once

#if defined(_WINDOWS_PLATFORM_)

#include <process.h>

typedef unsigned int(__stdcall *THREAD_FUNC) (void*);

typedef HANDLE						THREAD_HANDLE;
#define INVALID_THREAD_HANDLE       INVALID_HANDLE_VALUE
#define CREATE_THREAD(h, f, p)      h = (THREAD_HANDLE)_beginthreadex(NULL, 0, (THREAD_FUNC)f, p, 0, NULL)
#define WAIT_THREAD_END(x)			WaitForSingleObject(x, INFINITE)
#define END_THREAD(x)				TerminateThread((x), 0);

typedef HANDLE						MUTEX_HANDLE;
#define INVALID_MUTEX_HANDLE        INVALID_HANDLE_VALUE
#define CREATE_MUTEX(x)				(x) =  CreateMutex(NULL, FALSE, NULL)
#define LOCK_MUTEX(x)				WaitForSingleObject((x), INFINITE)
#define UNLOCK_MUTEX(mutex)			ReleaseMutex((mutex))
#define DESTROY_MUTEX(x)			CloseHandle((x))
#define MUTEX_INIT(x,v)				(x) = (v)

typedef HANDLE								SEMAPHORE_HANDLE;
#define INVALID_SEMAPHORE_HANDLE            INVALID_HANDLE_VALUE
#define CREATE_SEMAPHORE(x, name, initVal)	(x) = CreateSemaphoreA(NULL, initVal, 32767, name)
#define WAIT_SEMAPHORE(x)					WaitForSingleObject((x), INFINITE)
#define INCREASE_SEMAPHORE(x)				ReleaseSemaphore((x), 1, NULL);
#define DESTROY_SEMAPHORE(x, name)			CloseHandle((x))

//namespace Pharos
//{
//	namespace Utils
//	{
//		inline void SetCurrThreadName(const char8* szThreadName)
//		{
//			typedef struct tagTHREADNAME_INFO
//			{
//				DWORD dwType; // must be 0x1000
//				LPCSTR szName; // pointer to name (in user addr space)
//				DWORD dwThreadID; // thread ID (-1=caller thread)
//				DWORD dwFlags; // reserved for future use, must be zero
//			} THREADNAME_INFO;
//
//			THREADNAME_INFO info;
//			info.dwType = 0x1000;
//			info.szName = szThreadName;
//			info.dwThreadID = GetCurrentThreadId();
//			info.dwFlags = 0;
//
//			__try
//			{
//				RaiseException(0x406D1388, 0, sizeof(info) / sizeof(DWORD), (DWORD*)&info);
//			}
//			__except (EXCEPTION_CONTINUE_EXECUTION)
//			{
//			}
//		}
//	}
//}

#endif

#if defined(_IOS_PLATFORM_) || defined(_ANDROID_PLATFORM_) || defined(_MACOS_PLATFORM_)

#include <string.h>
#include <pthread.h>
#include <semaphore.h>

typedef pthread_t					THREAD_HANDLE;
#define INVALID_THREAD_HANDLE       NULL
#define CREATE_THREAD(h, f, p)      pthread_create(&h, NULL, f, p)
#define WAIT_THREAD_END(x)			pthread_join(x, NULL)
#define END_THREAD(x)				pthread_exit(x)


typedef pthread_mutex_t				MUTEX_HANDLE;
#define INVALID_MUTEX_HANDLE        (pthread_mutex_t){0}
#define CREATE_MUTEX(x)				pthread_mutex_init(&(x), NULL)
#define LOCK_MUTEX(x)				pthread_mutex_lock(&(x))
#define UNLOCK_MUTEX(mutex)			pthread_mutex_unlock(&(mutex))
#define DESTROY_MUTEX(x)			pthread_mutex_destroy(&(x))
#define MUTEX_INIT(x,v)				pthread_mutex_init(&(x),(v))


typedef sem_t*                              SEMAPHORE_HANDLE;
#define INVALID_SEMAPHORE_HANDLE            SEM_FAILED
#define CREATE_SEMAPHORE(x, name, initVal)	(x) = sem_open(name, O_CREAT|O_EXCL, 0644, initVal)
#define WAIT_SEMAPHORE(x)					sem_wait((x))
#define INCREASE_SEMAPHORE(x)				sem_post((x))
#define DESTROY_SEMAPHORE(x, name)			{sem_close(x);sem_unlink(name);}

//#define SetCurrThreadName(name)		(name = name);

#endif

namespace Pharos
{
	namespace Utils
	{
		class Mutex
		{
		public:
			Mutex()
			{
				m_hHandle = INVALID_MUTEX_HANDLE;

				CREATE_MUTEX(m_hHandle);

				m_bLock = false;
			}

			~Mutex()
			{
				DESTROY_MUTEX(m_hHandle);
			}

		private:
			MUTEX_HANDLE	m_hHandle;
			volatile bool	m_bLock;

		public:
			bool TryLock()
			{
				if (m_bLock) return false;

				LOCK_MUTEX(m_hHandle);
				m_bLock = true;

				return true;
			}

			void Lock()
			{
				LOCK_MUTEX(m_hHandle);
				m_bLock = true;
			}

			void Unlock()
			{
				UNLOCK_MUTEX(m_hHandle);
				m_bLock = false;
			}

			bool IsLock()
			{
				return m_bLock;
			}
		};

		class Semaphore
		{
		public:
			Semaphore(uint32 nInitValue, const char8* name)
			{
				m_hHandle = INVALID_SEMAPHORE_HANDLE;
				m_name = name;
				SetInitValue(nInitValue);
			}

			~Semaphore(void)
			{
				DESTROY_SEMAPHORE(m_hHandle, m_name.c_str());
				m_hHandle = NULL;
			}

		private:
			SEMAPHORE_HANDLE	m_hHandle;
			string              m_name;
			volatile uint32		m_nRefCount;

		public:
			void SetInitValue(uint32 nInitValue)
			{
				if (m_hHandle != INVALID_SEMAPHORE_HANDLE)
				{
					DESTROY_SEMAPHORE(m_hHandle, m_name.c_str());
				}

				CREATE_SEMAPHORE(m_hHandle, m_name.c_str(), nInitValue);

				if (m_hHandle == INVALID_SEMAPHORE_HANDLE)
				{
					DESTROY_SEMAPHORE(m_hHandle, m_name.c_str());
					CREATE_SEMAPHORE(m_hHandle, m_name.c_str(), nInitValue);
				}

				m_nRefCount = nInitValue;
			}

			void WaitAndDecrease()
			{
				WAIT_SEMAPHORE(m_hHandle);
				m_nRefCount--;
			}

			void IncreaseRefCount()
			{
				INCREASE_SEMAPHORE(m_hHandle);
				m_nRefCount++;
			}

			uint32 GetSemaphoreRefCount()
			{
				return m_nRefCount;
			}
		};
	}
}

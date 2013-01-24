/********************************************************************
	created:	2012/09/19
	filename: 	Thread.h
	author:		reedhong
	
	purpose:	线程
*********************************************************************/

#ifndef __Lotus2d_Thread_H__
#define __Lotus2d_Thread_H__

#include "Base/Prerequisites.h"

#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_WIN32
#include <process.h>
#else 
#include <pthread.h>
#endif

namespace Lotus2d {

	/*线程*/
	class Thread
	{
	public:
		Thread():m_pid(-1)  ,m_isRun(false),m_data(NULL)
		  {
		  }
		  ~Thread()
		  {
			  close();
		  }

		  void create(unsigned long (*start_address)(void*), void* p)
		  {
#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_WIN32
			  m_isRun = TRUE;
			  HANDLE hThread = (HANDLE)_beginthreadex( NULL,0, (unsigned (__stdcall *)(void*))start_address, p, 0, (unsigned *)&m_pid );
			  if (hThread){
				  CloseHandle(hThread);
			  }else{
				  m_isRun = FALSE;
			  }
#else
			  m_isRun = true;
			  pthread_t pid;
			  pthread_attr_t pattr;
			  pthread_attr_init(&pattr);
			  pthread_attr_setdetachstate(&pattr,PTHREAD_CREATE_DETACHED);

			  pthread_create(&pid, &pattr,(void*(*)(void *))start_address, p);
			  m_pid= (int)pid;
			  pthread_attr_destroy(&pattr);
			  if (m_pid == -1) //returns –1L on an error
				  m_isRun = false;
#endif
		  }

		  void close()
		  {
			  if (!m_isRun)
				  return;
			  m_isRun = false; //wish the thread would end itself
			  m_pid = -1;
		  }

		  inline BOOL isRun() { return m_isRun; }

		  static void sleep(unsigned long ms)
		  {
#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_WIN32
			  ::Sleep(ms);
#else
			  struct timespec tp;
			  tp.tv_sec = (int) ms/1000;
			  tp.tv_nsec = ms%1000;
			  nanosleep(&tp, &tp);
#endif
		  }

#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_WIN32
		  static DWORD        getCurrentThreadID() { return ::GetCurrentThreadId(); }
#else    
		  static  pthread_t   getCurrentThreadID() { return ::pthread_self(); }
#endif

		  void*   m_data;       
		  int     m_pid;

		  BOOL    m_isRun;
	};

	/*互斥对象*/
	class Mutex
	{
	public:

		Mutex();
		~Mutex();

		inline void lock() 
		{
#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_WIN32
			::EnterCriticalSection(&fMutex);
#else
			(void)pthread_mutex_lock(&fMutex);
#endif
		};

		inline void unlock() 
		{
#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_WIN32
			::LeaveCriticalSection(&fMutex);
#else
			pthread_mutex_unlock(&fMutex);
#endif
		};

	private:

#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_WIN32
		CRITICAL_SECTION fMutex;                     
#else
		pthread_mutex_t fMutex;       
#endif       
	};

	/*锁，在Mutex之上做简单封装，用于自动加锁和解锁*/
	class Lock
	{
	public:
		Lock(Mutex& cs) : _cs(cs)
		{
			_cs.lock();
		}
		~Lock()
		{
			_cs.unlock();
		}
	private:
		Mutex& _cs;
	};

#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_WIN32
	/*信号量*/
	class Semaphore
	{
	public:

		/**
		* @param lInitialCount 信号量的初始值,必须大于等于0, 小于等于lMaximumCount
		* @param lMaximumCount 信号量的最大值,必须大于等于0
		* @param lpName 信号量的名字
		*/
		Semaphore(LONG lInitialCount = 0, LONG lMaximumCount = MAXLONG, LPCTSTR lpName = NULL)
		{
			h = ::CreateSemaphore( NULL,
				lInitialCount,
				lMaximumCount,
				lpName
				);
		}

		/**
		* 增加一个可进入
		*/
		bool signal()
		{
			if (h)
				return ReleaseSemaphore(h, 1, NULL)?true:false;
			else
				return false;
		}

		bool wait()
		{
			if (h)
				return (WaitForSingleObject(h, INFINITE) == WAIT_OBJECT_0);
			else
				return false;
		}

		bool wait(unsigned int msecond)
		{
			if (h)
				return WaitForSingleObject(h, msecond) == WAIT_OBJECT_0;
			else
				return false;
		}

		void close()
		{
			if(h)
			{
				CloseHandle(h);
				h = NULL;
			}
		}
		~Semaphore()
		{
			close();
		}

	private:
		HANDLE h;

	public:
		Semaphore(const Semaphore&);
		Semaphore& operator=(const Semaphore&);
	};
#endif
}


#endif

/********************************************************************
	created:	2012/09/20
	filename: 	Thread.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "Thread.h"

namespace Lotus2d {

#if LOTUS2D_PLATFORM != LOTUS2D_PLATFORM_WIN32
	static pthread_mutexattr_t  *sMutexAttr=NULL;
	static void MutexAttrInit();
	static pthread_once_t sMutexAttrInit = PTHREAD_ONCE_INIT;
#endif

	Mutex::Mutex()
	{
#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_WIN32
		::InitializeCriticalSection(&fMutex);
#else
		(void)pthread_once(&sMutexAttrInit, MutexAttrInit);
		(void)pthread_mutex_init(&fMutex, sMutexAttr);
#endif
	}

#if LOTUS2D_PLATFORM != LOTUS2D_PLATFORM_WIN32
	void MutexAttrInit()
	{
		sMutexAttr = (pthread_mutexattr_t*)malloc(sizeof(pthread_mutexattr_t));
		::memset(sMutexAttr, 0, sizeof(pthread_mutexattr_t));
		pthread_mutexattr_init(sMutexAttr);
	}
#endif

	Mutex::~Mutex()
	{
#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_WIN32
		::DeleteCriticalSection(&fMutex);
#else
		pthread_mutex_destroy(&fMutex);
#endif
	}


}
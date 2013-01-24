/********************************************************************
	created:	2012/09/19
	filename: 	Memory.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "Memory.h"
#include "Assert.h"
#include "Thread/Thread.h"

namespace Lotus2d {

#ifdef MEMORY_DEBUG
	static Mutex s_mutex;
	void Memory::debug_malloc( MemoryCookie* mc)
	{	
		// 暂时不考虑加锁的事情
		MemoryCookie* tmp ;	
		mId++;
		mc->id = mId;
		assert(mc->id != mBreakId);
		Lock lock(s_mutex);
		if(mHeader == NULL){  
			mHeader = m_guard+0;
			mHeader->size = 0;
			mHeader->id = -1;

			tmp = m_guard+1;
			memset(tmp, 0, sizeof(mHeader));
			tmp->size = 0;
			tmp->id = -1;

			mHeader->next = mHeader->last = tmp;
			tmp->next = tmp->last = mHeader;
		}
		mc->last = mHeader;
		mc->next = mHeader->next;
		mHeader->next->last = mc;
		mHeader->next = mc;
		mTotalMemory += mc->size;
	}

	void Memory::debug_free(struct  MemoryCookie* mc)
	{
		ASSERT(mc->id != -1);
		Lock lock(s_mutex);
		mc->last->next = mc->next;
		mc->next->last = mc->last;
		mc->id = -1;
		mTotalMemory -= mc->size;
	}

	Memory::Memory()
	{
		mTotalMemory =0;
		mTotalMalloc =0;
		mTotalFree =0;
		mTotalRealloc =0;
		mHeader =0;

		mId =0;
		mBreakId =0;
		//setBreak(194);
	}

	Memory::~Memory()
	{
		check();
	}

	void *Memory::memory_malloc(size_t size)
	{
		void *ptr;

		MemoryCookie* mc;

		mTotalMalloc++;
		size += sizeof(struct MemoryCookie);
		ptr = malloc(size);	
		ASSERT(ptr != NULL);

		mc = (struct MemoryCookie *)ptr;
		mc->size = size;
		debug_malloc(mc);
		return (mc+1);

	}

	Memory* Memory::Instance()
	{
		static Memory s_instance;
		return &s_instance;
	}

	void Memory::memory_free(void* ptr)
	{
		MemoryCookie* mc;
		if (ptr == NULL){
			return;
		}
		mTotalFree++;
		mc = (MemoryCookie *)ptr;
		debug_free(mc-1);
		free(mc-1);
	}


	void* Memory::memory_realloc(void *ptr, size_t newsize)
	{
		void *newptr;
		MemoryCookie* mc;
		ASSERT(ptr!=NULL || newsize !=0);

		mTotalRealloc++;
		mc = (MemoryCookie *)ptr;
		debug_free(mc-1);

		newsize += sizeof(MemoryCookie);
		newptr = realloc(mc-1, newsize);
		ASSERT(newptr != NULL);
		mc = (MemoryCookie *)newptr;
		mc->size = newsize;
		debug_malloc(mc);
		return mc+1;
	}

	void Memory::check()
	{
		MemoryCookie* iter;
		int count = 0;
		//ge_require_lock(&s_memoryLock);
		if(mHeader  == NULL)
			return ;
		iter = mHeader->next;	
		while( iter != mHeader->last){
			//ge_assert( iter->id>=0 && iter->id < mId);
			iter = iter->next;
			count ++;
		}
		//ge_release_lock(&s_memoryLock);
		LOGD("memory need free count= %d, malloc=%d, free=%d, realloc=%d!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!", 
			count, mTotalMalloc, mTotalFree, mTotalRealloc);
	}

	void Memory::setBreak(size_t id)
	{
		mBreakId = id;
	}
#endif
}





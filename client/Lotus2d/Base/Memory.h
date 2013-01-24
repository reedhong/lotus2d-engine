/********************************************************************
	created:	2012/09/19
	filename: 	Memory.h
	author:		reedhong
	
	purpose:	内存分配的检测，用于debug的情况
*********************************************************************/

#ifndef __Lotus2d_Memroy_H__
#define __Lotus2d_Memroy_H__


//#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "Base/Config.h"



namespace Lotus2d {
#ifdef MEMORY_DEBUG	
	struct MemoryCookie{
		struct MemoryCookie*	last;
		struct MemoryCookie*	next;
		size_t	 size;
		size_t id;
	};


	class Memory
	{
	protected:
		Memory();
	public:
		~Memory();
		static Memory* Instance();
	public:
		void* memory_malloc(size_t size);
		void memory_free(void* ptr);
		void* memory_realloc(void* ptr, size_t size_t);

		void check();
		void setBreak(size_t id);
	private:
		void debug_malloc(MemoryCookie* mc);
		void debug_free(MemoryCookie* mc);
	private:
		size_t mTotalMemory;
		size_t mTotalMalloc;
		size_t mTotalFree;
		size_t mTotalRealloc;

		MemoryCookie* mHeader;
		MemoryCookie	m_guard[2];

		size_t mId;
		size_t mBreakId;
	};

#define lotus2d_malloc(size)		Memory::Instance()->memory_malloc(size)
#define lotus2d_free(ptr)	Memory::Instance()->memory_free(ptr)
#define lotus2d_realloc(ptr, size)		Memory::Instance()->memory_realloc(ptr, size)

#else
#define lotus2d_malloc(size)		malloc(size)
#define lotus2d_free(ptr)		free(ptr)
#define lotus2d_realloc(ptr, size)		realloc(ptr, size)
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(x)     if( (x)!=NULL ) { delete (x); (x)=NULL; }
#endif
	//根据指针值删除数组类型内存
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x)     if( (x)!=NULL ) { delete[] (x); (x)=NULL; }
#endif
	//根据指针调用free接口
#ifndef SAFE_FREE
#define SAFE_FREE(x)     if( (x)!=NULL ) { lotus2d_free(x); (x)=NULL; }
#endif
	//根据指针调用Release接口
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)     if( (x)!=NULL ) { (x)->release(); (x)=NULL; }
#define SAFE_ADDREF(x)	  if( (x)!=NULL ) { (x)->addRef();}	
#endif

}

#endif

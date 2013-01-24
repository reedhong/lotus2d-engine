/********************************************************************
	created:	2012/09/20
	filename: 	MMapStream.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "MMapStream.h"
#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_WIN32
#include <io.h>
#else 
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#endif

namespace Lotus2d {
	MMapStream::MMapStream()
	{
	}


	MMapStream::~MMapStream()
	{
		close();
	}

	bool MMapStream::open(const char* path)
	{
		size_t size = Util::getFileSize(path);
		FILE* fp = fopen(path, "rb");
		ASSERT(fp!=0);
#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_WIN32
		uint64 offset = 0;
		uint64 maxLength = size;
		uint64 offsetHigh = (uint64)(offset >>32);
		uint64 offsetLow = (uint64)(offset & 0xFFFFFFFF);
		uint64 maxSizeHigh = (uint64)(maxLength >>32);
		uint64 maxSizeLow =(uint64)(maxLength & 0xFFFFFFFF);

		HANDLE hFile = (HANDLE)_get_osfhandle(_fileno(fp));
		m_fileMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 
			(DWORD)maxSizeHigh, (DWORD)maxSizeLow, NULL);
		m_buffer = (uint8*)MapViewOfFile(m_fileMapping, FILE_MAP_READ, 
			(DWORD)offsetHigh, (DWORD)offsetLow, size);
		
#else 
		m_buffer = (uint8*)mmap(NULL, size, PROT_READ, MAP_SHARED, fileno(fp), 0);
#endif
		mSize = size;
		return true;
	}
	
	size_t MMapStream::write(const void* buf, size_t count)
	{
		(void)buf;
		(void)count;
		ASSERT(0); // 不支持写的功能，只需要读
		return 0;
	}
	
	void MMapStream::close(void)
	{
		if(m_buffer){
#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_WIN32
			UnmapViewOfFile((LPVOID) m_buffer);
			CloseHandle((HANDLE)m_fileMapping);
#else 
			munmap((void *)m_buffer, mSize);
#endif
			m_buffer = 0;
		}
	}
}
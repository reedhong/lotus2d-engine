/********************************************************************
	created:	2012/09/20
	filename: 	MMapStream.h
	author:		reedhong
	
	purpose:	mmap∂¡»°
*********************************************************************/

#ifndef __Lotus2d_MMapStream_H__
#define __Lotus2d_MMapStream_H__

#include "Base/Prerequisites.h"
#include "Base/MemoryStream.h"

namespace Lotus2d {
	class MMapStream: public MemoryStream
	{
	public:
		MMapStream();
		virtual ~MMapStream();

		bool open(const char* path);
		virtual size_t write(const void* buf, size_t count);
		virtual void close(void);
	private:
#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_WIN32
		void*		m_fileMapping;
#endif
	};
}

#endif
/********************************************************************
	created:	2012/09/14 
	filename: 	FileStream.h
	author:		reedhong 
	
	purpose:	调用c的接口，尽量减少对std的使用
*********************************************************************/

#ifndef __Lotus2d_FileStream_H__
#define __Lotus2d_FileStream_H__

#include "Stream.h"

namespace Lotus2d {
	class FileStream: public Stream
	{
	public:
		/*
		 * mode的值跟c的fopen保持一致
		 */
		FileStream(const char* path,  const char* mode);
		virtual ~FileStream();
		
		virtual size_t read(void* buf, size_t count);
		virtual size_t write(const void* buf, size_t count);
		virtual void skip(long count);
		virtual void seek( size_t pos );
		virtual size_t tell(void);
		virtual bool eof(void)  const;
		virtual void close(void);

	private:
		FILE*	m_file;
	};
}

#endif
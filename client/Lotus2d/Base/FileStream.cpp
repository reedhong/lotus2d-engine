/********************************************************************
	created:	2012/09/14 
	filename: 	FileStream.cpp
	author:		reedhong 
	
	purpose:  
*********************************************************************/

#include "FileStream.h"

namespace Lotus2d {
	FileStream::FileStream(const char* path, const char* mode)
	{
		m_file = fopen(path, mode);
		ASSERT(m_file!=NULL);
	}

	FileStream::~FileStream()
	{

	}

	size_t FileStream::read(void* buf, size_t count)
	{
		return fread(buf, 1, count,	m_file);
	}

	size_t FileStream::write(const void* buf, size_t count)
	{
		return fwrite(buf, 1, count, m_file);
	}

	void FileStream::skip(long count)
	{
		fseek(m_file, count, SEEK_CUR);
	}

	void FileStream::seek( size_t pos )
	{
		fseek(m_file, static_cast<long>(pos), SEEK_SET);
	}

	size_t FileStream::tell(void)
	{
		return ftell(m_file);
	}

	bool FileStream::eof(void) const 
	{	
		return feof(m_file) != 0;
	}

	void FileStream::close(void)
	{
		if(m_file){
			fclose(m_file);
			m_file = NULL;
		}
	}
}
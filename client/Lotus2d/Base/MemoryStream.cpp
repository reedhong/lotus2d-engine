/********************************************************************
	created:	2012/09/14 
	filename: 	MemroyStream.cpp
	author:		reedhong 
	
	purpose:	
*********************************************************************/

#include "MemoryStream.h"

namespace Lotus2d {
	MemoryStream::MemoryStream():m_buffer(0),m_bufferStart(0),m_bufferEnd(0),m_freeOnClose(false)
	{

	}

	MemoryStream::MemoryStream(const uint8 *buffer, size_t len, bool freeOnClose):m_freeOnClose(freeOnClose)
	{
		m_buffer =  (uint8 *)buffer;
		m_bufferStart = (uint8 *)buffer;
		m_bufferEnd = (uint8 *)buffer + len;
		mSize = len;
		m_length = len;
	}

	MemoryStream::~MemoryStream()
	{
		m_buffer = m_bufferStart = m_bufferEnd  = 0;
		close();
	}

	size_t MemoryStream::read(void* buf, size_t count)
	{
		ASSERT(count <= (size_t)(m_bufferEnd-m_buffer));
		memcpy(buf, m_buffer, count);
		m_buffer += count;

		return count;
	}

	void MemoryStream::setSize(size_t size)
	{
		ASSERT(m_length >= size);
		mSize = size; 
		seek(0);
		m_bufferEnd = (uint8 *)m_buffer + mSize;
	}

	size_t MemoryStream::write(const void* buf, size_t count)
	{
		ASSERT(count <= (size_t)(m_bufferEnd-m_buffer));
		memcpy(m_buffer, buf, count);
		m_buffer += count;

		return count;
	}
	void MemoryStream::skip(long count)
	{
		ASSERT(count <= (m_bufferEnd-m_buffer));
		m_buffer += count;
	}
	void MemoryStream::seek( size_t pos )
	{
		ASSERT(pos <= (size_t)(m_bufferEnd-m_bufferStart));
		m_buffer = m_bufferStart+pos;
	}
	size_t MemoryStream::tell(void)
	{
		return (m_buffer-m_bufferStart);
	}
	bool MemoryStream::eof(void) const 
	{
		return (m_buffer == m_bufferEnd);
	}
	void MemoryStream::close(void)
	{
		if(m_freeOnClose && m_bufferStart){
			lotus2d_free(m_bufferStart);
		}
	}
}
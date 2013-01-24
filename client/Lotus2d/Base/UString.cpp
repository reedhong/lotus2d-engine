/********************************************************************
	created:	2012/09/17
	filename: 	UString.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "UString.h"

namespace Lotus2d {


	/* Judge if the specified utf-8 character's head start is valid. */
#define UTF8_LEGAL_STARTCHAR(src)		\
	((src[0] & 0x80) == 0 ||		\
	((src[0] & 0xFF) >= 0xC2 &&		\
	(src[0] & 0xFF) <= 0xDF) ||		\
	(src[0] & 0xF0) == 0xE0 ||		\
	(src[0] & 0xFF) == 0xF0)

	/* Judge the len of bytes with the specified utf-8 character */
#define UTF8_SEQ_LENGTH(src)			\
	((src[0] & 0x80) == 0 ? 1 : 		\
	(((src[0] & 0xFF) >= 0xC2 &&		\
	(src[0] & 0xFF) <= 0xDF) ? 2 : 	\
	((src[0] & 0xF0) == 0xE0 ? 3 : 	\
	((src[0] & 0xFF) == 0xF0 ? 4 : 0))))

	/* Judge if the specified utf-8 character is valid. */
#define UTF8_SEQ_VALID(src)			\
	((src[0] & 0x80) == 0 ? 1 : 		\
	(((src[0] & 0xFF) >= 0xC2 &&		\
	(src[0] & 0xFF) <= 0xDF &&		\
	(src[1] & 0xC0) == 0x80) ? 2 : 	\
	((src[0] & 0xF0) == 0xE0 &&		\
	(src[1] & 0xC0) == 0x80 &&		\
	(src[2] & 0xC0) == 0x80 ? 3 : 	\
	((src[0] & 0xFF) == 0xF0 &&		\
	(src[1] & 0xC0) == 0x80 &&		\
	(src[2] & 0xC0) == 0x80 &&		\
	(src[3] & 0xC0) == 0x80 ? 4 : 0))))

	/* Get the character code of the specified utf-8 character */
#define UTF8_GET_CHAR(src)			\
	((src[0] & 0x80) == 0 ? src[0] : 	\
	(((src[0] & 0xFF) >= 0xC2 &&		\
	(src[0] & 0xFF) <= 0xDF &&		\
	(src[1] & 0xC0) == 0x80) ?		\
	((src[0] & 0x1F) << 6) |		\
	(src[1] & 0x3F) : 		\
	((src[0] & 0xF0) == 0xE0 &&		\
	(src[1] & 0xC0) == 0x80 &&		\
	(src[2] & 0xC0) == 0x80 ?		\
	((src[0] & 0x0F) << 12) |		\
	((src[1] & 0x3F) << 6) |		\
	(src[2] & 0x3F) :			\
	((src[0] & 0xFF) == 0xF0 &&		\
	(src[1] & 0xC0) == 0x80 &&		\
	(src[2] & 0xC0) == 0x80 &&		\
	(src[3] & 0xC0) == 0x80 ?		\
	((src[1] & 0x3F) << 12) |		\
	((src[2] & 0x3F) << 6) |		\
	(src[3] & 0x3F) : -1))))

	/* Get the len of bytes in the format of utf-8 of the specified character code */
#define UTF8_CHAR_LENGTH(ch)			\
	((ch >= 0x00000 && ch <= 0x0007F) ? 1 : \
	(ch >= 0x00080 && ch <= 0x007FF) ? 2 :	\
	(ch >= 0x00800 && ch <= 0x0FFFF) ? 3 :	\
	(ch >= 0x10000 && ch <= 0x3FFFF) ? 4 : 1) //FIXME HERE. 

	/* Put the specified character code to the specified utf-8 coded string */
#define UTF8_PUT_CHAR(dest, ch)				\
	((ch >= 0x00000 && ch <= 0x0007F) ? 		\
	(dest[0] = ch) :				\
	(ch >= 0x00080 && ch <= 0x007FF) ? 		\
	((dest[0] = ((ch >>  6) & 0x1F) | 0xC0),	\
	(dest[1] = ( ch        & 0x3F) | 0x80)) :	\
	(ch >= 0x00800 && ch <= 0x0FFFF) ? 		\
	((dest[0] = ((ch >> 12) & 0x0F) | 0xE0),	\
	(dest[1] = ((ch >>  6) & 0x3F) | 0x80),	\
	(dest[2] = ( ch        & 0x3F) | 0x80)) :	\
	(ch >= 0x10000 && ch <= 0x3FFFF) ? 		\
	((dest[0] = 0xF0),				\
	(dest[1] = ((ch >> 12) & 0x3F) | 0x80),	\
	(dest[2] = ((ch >>  6) & 0x3F) | 0x80),	\
	(dest[3] = ( ch        & 0x3F) | 0x80)) : 0)



	UString::UString()
	{
		init(NULL, 0);
	}

	UString::UString(const char* buf)
	{
		init(buf, strlen(buf));
	}

	UString::UString(const char* buf, size_t len)
	{
		init(buf, len);
	}

	UString::UString(char c, size_t count)
	{
		char* buf = (char*)lotus2d_malloc(count+1);
		memset(buf, c, count);
		buf[count] = '\0';
		init(buf, count);
		lotus2d_free(buf);
	}

	UString::UString(const UString& str)
	{
		init(str.c_str(), str.size());
	}

	UString::~UString()
	{
		if( m_mallocBuffer != m_localBuffer){
			SAFE_FREE(m_mallocBuffer);
		}
	}

	UString& UString::operator= (const UString& str)
	{
		if (this==&str) return *this;  
		init(str.c_str(), str.size());
		return *this;
	}

	UString& UString::operator= (const char* str)
	{
		init(str, strlen(str));
		return *this;
	}

	size_t UString::length() const
	{
		return m_strLen;
	}

	size_t UString::size() const
	{
		return m_mallocBufferLen;
	}

	bool UString::empty() const
	{
		return m_mallocBufferLen==0;
	}

	const char* UString::c_str() const
	{
		return m_mallocBuffer;
	}


	int UString::get(size_t index) const
	{
		char* str = m_mallocBuffer;

		if(!m_strLen){
			return -1;
		}

		while (index > 0){
			int ch = UTF8_GET_CHAR(str);
			if (!ch){
				return -1;
			}

			str += UTF8_CHAR_LENGTH(ch);
			index--;
		}

		return UTF8_GET_CHAR(str);
	}

	int UString::getUnicodeFromIterator()
	{
		char* str = m_mallocBuffer+m_iter;
		if(!(*str)){
			m_iter = 0;
			return -1;
		}
		int ch = UTF8_GET_CHAR(str);
		if (!ch){
			m_iter = 0;
			return -1;
		}
		m_iter += UTF8_CHAR_LENGTH(ch);
		return ch;
	}

	void UString::init(const char* buf, size_t len)
	{
		m_localBuffer[0]='\0';
		m_mallocBuffer = m_localBuffer;		// stream data width utf-8 format.
		m_mallocBufferLen = 0;	// len of stream data.
		m_strLen = 0;		// len of string.
		m_iter =0;
		if(buf == NULL)
			return;

		/* Init the string */
		m_mallocBufferLen = len;
		if(len > LocalFixStringSize){
			m_mallocBuffer = (char *)lotus2d_malloc(len+1);
		}
		memcpy(m_mallocBuffer, buf, len);
		m_mallocBuffer[len] = 0;
		m_strLen = _utf_string_len(m_mallocBuffer, m_mallocBufferLen);
	}

	int UString::_utf_string_len(const char* buf, size_t size)
	{
		int len = 0;
		int i = 0;
		if (buf == NULL){
			return 0;
		}
		while(i < (int)size){
			int sl = UTF8_SEQ_LENGTH(buf);
			if (sl){
				len++;
				buf += sl;
			}
			else{  // Something wrong here, failure recover.
				// utf8_data++;
				break;
			}
			i += sl;
		}

		if(i > (int)size){ // Overflow, invalid data stream.
			return -1;
		}

		return len;
	}

	UString UString::substr(size_t startIndex, size_t endIndex) const 
	{
		endIndex = endIndex > m_strLen ? m_strLen : endIndex;

		UString sRet;
		const char* str = (const char *)m_mallocBuffer;
		if(endIndex - startIndex != 0){
			size_t n = 0;
			int start = -1, end = -1, index = 0;
			while(n < m_mallocBufferLen){
				int len = UTF8_SEQ_LENGTH(str);
				if(index == startIndex){
					start = n;
				}
				if(index == endIndex - 1){
					end = n + len;
					break;
				}

				n += len;
				str += len;
				index++;
			}

			if(start != -1 && end != -1){
				sRet.init( (const char*)(m_mallocBuffer+start), end-start);
			}
		}

		return sRet;
	}

	void UString::append(char c)
	{
		append(&c, 1);
	}

	void UString::append(const char* str)
	{
		append(str, strlen(str));
	}

	void UString::append(const char* buffer, size_t len)
	{
		if( m_mallocBuffer == m_localBuffer && len<=(LocalFixStringSize-m_mallocBufferLen)){
			memcpy(m_mallocBuffer+m_mallocBufferLen, buffer, len);
			m_mallocBufferLen += len;
		}else{
			char* newBuffer = (char *)lotus2d_malloc(m_mallocBufferLen+len+1);
			memcpy(newBuffer, m_mallocBuffer, m_mallocBufferLen);
			memcpy(newBuffer+m_mallocBufferLen, buffer, len);
			m_mallocBufferLen += len;
			newBuffer[m_mallocBufferLen] = '\0';
			if(m_mallocBuffer != m_localBuffer)
				SAFE_FREE(m_mallocBuffer);
			m_mallocBuffer = newBuffer;
		}
		m_strLen = _utf_string_len(m_mallocBuffer, m_mallocBufferLen);
	}
}
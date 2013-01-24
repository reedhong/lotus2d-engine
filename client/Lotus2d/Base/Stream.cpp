/********************************************************************
	created:	2012/09/14 
	filename: 	Stream.cpp
	author:		reedhong 
	
	purpose:	
*********************************************************************/

#include "Stream.h"

namespace Lotus2d {
	Stream::Stream()
	{

	}

	Stream::~Stream()
	{

	}

	void Stream::swapEndian(void* pData, size_t size)
	{
		unsigned char swapByte;
		for(unsigned int byteIndex = 0; byteIndex < size/2; byteIndex++)
		{
			swapByte = *(char *)((size_t)pData + byteIndex);
			*(unsigned char *)((size_t)pData + byteIndex) = *(unsigned char *)((size_t)pData + size - byteIndex - 1);
			*(unsigned char *)((size_t)pData + size - byteIndex - 1) = swapByte;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// read
	BOOL Stream::readBool()
	{
		uint8 c;
		read(&c, 1);
		return c==TRUE;
	}
	int8 Stream::read8()
	{
		int8 c;
		read(&c, 1);
		return c;
	}
	int16 Stream::read16()
	{
		int16 s;
		read(&s, 2);
		swapEndian(&s, 2);
		return s;
	}

	int32 Stream::read32()
	{
		int32 i;
		read(&i, 4);
		swapEndian(&i, 4);
		return i;
	}

	int64 Stream::read64()
	{
		int64 l;
		read(&l, sizeof(int64));
		swapEndian(&l, sizeof(int64));
		return l;
	}

	float Stream::readf()
	{
		float f;
		read(&f, sizeof(float));
		return f;
	}

	UString Stream::readUString(bool isShort)
	{	
		int len = 0;
		if( isShort){
			len = read16();
		}else{
			len = read8();
		}
		char* buf = (char *)lotus2d_malloc(len+1);
		read((char *)buf, len);
		buf[len] = '\0';
		UString sRet(buf, len);
		lotus2d_free(buf);
		return sRet;
	}

	int16 Stream::read16le()
	{
		int16 s;
		read(&s, 2);
		return s;
	}

	int32 Stream::read32le()
	{
		int32 i;
		read(&i, 4);
		return i;
	}
	int64 Stream::read64le()
	{
		int64 l;
		read(&l, sizeof(int64));
		return l;
	}

	//////////////////////////////////////////////////////////////////////////
	// write
	void Stream::writeBool(BOOL b)
	{
		write8(b);
	}
	void Stream::write8(int8 c)
	{
		write(&c, 1);
	}

	void Stream::write16(int16 s)
	{
		swapEndian(&s, 2);
		write(&s, 2);
	}

	void Stream::write32(int32 i)
	{
		swapEndian(&i, 4);
		write(&i, 4);
	}

	void Stream::write64(int64 l)
	{
		swapEndian(&l, sizeof(int64));
		write(&l, sizeof(int64));
	}

	void Stream::writef(float f)
	{
		write(&f, sizeof(float));
	}

	void Stream::writeUString(const UString& str, bool isShort)
	{
		size_t strLen = str.length();
		if( isShort){
			write16(strLen);
		}else{
			write8(strLen);
		}
		if(strLen > 0){
			write(str.c_str(), strLen);
		}
	}

	void Stream::write16le(int16 s)
	{
		write(&s, 2);
	}

	void Stream::write32le(int32 i)
	{
		write(&i, 4);
	}

	void Stream::write64le(int64 l)
	{
		write(&l, sizeof(int64));
	}
}
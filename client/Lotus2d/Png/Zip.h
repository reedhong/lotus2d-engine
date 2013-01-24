/********************************************************************
	created:	2012/09/14 
	filename: 	Zip.h
	author:		reedhong 
	
	purpose:	裁剪的Zip解包，用于Png图片数据解析
*********************************************************************/

#ifndef __Lotus2d_Zip_H__
#define __Lotus2d_Zip_H__

#include "Base/Prerequisites.h"

namespace Lotus2d {
	class Zip
	{
		// ZLIB client - used by PNG, available for other purposes
	public:
		static char *ge_zlib_decode_malloc_guesssize(const char *buffer, int len, int initial_size, int *outlen);
		static char *ge_zlib_decode_malloc(const char *buffer, int len, int *outlen);
		static  int   ge_zlib_decode_buffer(char *obuffer, int olen, const char *ibuffer, int ilen);

		//char *ge_zlib_decode_noheader_malloc(const char *buffer, int len, int *outlen);
		//int   ge_zlib_decode_noheader_buffer(char *obuffer, int olen, const char *ibuffer, int ilen);
	};
}

#endif


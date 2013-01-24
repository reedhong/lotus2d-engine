/********************************************************************
	created:	2012/09/16
	filename: 	Util.h
	author:		reedhong
	
	purpose:	一些功能函数
*********************************************************************/

#ifndef __Lotus2d_Util_H__
#define __Lotus2d_Util_H__

#include "Prerequisites.h"

namespace Lotus2d {
	class Util
	{
	public:
		/*
		* 获取路径中的文件名，剔除掉后缀名
		 */
		static char* getFileNameNoExt(char* szPath);

		static size_t getFileSize(const char* path);


		static void geoTransform(int *px, int *py, int *pwidth, int *pheight, int *protateDegree, bool *pmirror, int trans, int anchor);

		static bool isInRect(int px,int py,int x,int y,int width,int height);

		static bool String2Buf(const string& str, unsigned char* buf, int len);

        /** Method for standardising paths - use forward slashes only, end with slash.
        */
        static string standardisePath( const string &init, bool endSlash = FALSE);

		static bool isExistedPath(const char* path);
	};
}

#endif
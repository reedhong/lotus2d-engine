/********************************************************************
	created:	2012/09/19
	filename: 	Log.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include <stdio.h>
#include <stdarg.h>

#include "Log.h"

#define MAX_LEN 1024

#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_WIN32
	#include <Windows.h>

	void winlog(const char * pszFormat, ...)
	{
		static char szBuf[MAX_LEN+1];
		va_list Args;
		szBuf[MAX_LEN] = '\0';
		va_start(Args, pszFormat);
		_vsnprintf(szBuf, MAX_LEN, pszFormat, Args);
		va_end(Args);

		OutputDebugStringA(szBuf);
		OutputDebugStringA("\n");
	}

#elif LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_ANDROID

	void androidlog(int logPriority,const char * pszFormat, ...)
	{
		static char szBuf[MAX_LEN+1];
		va_list args;
		szBuf[MAX_LEN] = '\0';
		va_start(args, pszFormat);    	
		vsnprintf(szBuf,MAX_LEN, pszFormat, args);
		va_end(args);

		__android_log_print(logPriority, "Lotus-Game",  szBuf);
	}

#elif LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_IOS
#define fmt_line "; line num= %d;\n"

	void ioslog(const char* func,int line,const char * pszFormat, ...)
	{
		static char szBuf[2*MAX_LEN+1];
		int nlen = 0;
		va_list args;
		szBuf[2*MAX_LEN] = '\0';
		//nlen += snprintf(szBuf,MAX_LEN*2,"%s", func);
		//nlen += snprintf(&szBuf[nlen],MAX_LEN*2,fmt_line,line);
		va_start(args, pszFormat);    	
		vsnprintf(&szBuf[nlen],MAX_LEN*2, pszFormat, args);
		va_end(args);

		printf("%s\n",szBuf);
	}

#endif
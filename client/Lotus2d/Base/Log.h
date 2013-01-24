/********************************************************************
	created:	2012/09/19
	filename: 	Log.h
	author:		reedhong
	
	purpose:	
*********************************************************************/

#ifndef __Lotus2d_Log_H__
#define __Lotus2d_Log_H__

#include "Platform/Platform.h"
#include "Base/Config.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LOTUS2D_DEBUG
	#if LOTUS2D_PLATFORM ==LOTUS2D_PLATFORM_ANDROID
		#include <android/log.h>
		void androidlog(int logPriority,const char * pszFormat, ...);	
		#define LOGV(fmt,...) androidlog(ANDROID_LOG_VERBOSE, fmt,__VA_ARGS__)
		#define LOGD(fmt,...) androidlog(ANDROID_LOG_ERROR  , fmt,__VA_ARGS__)
		#define LOGI(fmt,...) androidlog(ANDROID_LOG_INFO   , fmt,__VA_ARGS__)
		#define LOGW(fmt,...) androidlog(ANDROID_LOG_WARN   , fmt,__VA_ARGS__)
		#define LOGE(fmt,...) androidlog(ANDROID_LOG_ERROR  , fmt,__VA_ARGS__)
	#elif LOTUS2D_PLATFORM ==LOTUS2D_PLATFORM_WIN32
		void winlog(const char * pszFormat, ...);
		#define LOGV(fmt,...) winlog(fmt,  __VA_ARGS__)
		#define LOGD(fmt,...) winlog(fmt  ,__VA_ARGS__)
		#define LOGI(fmt,...) winlog(fmt  ,__VA_ARGS__)
		#define LOGW(fmt,...) winlog(fmt   ,__VA_ARGS__)
		#define LOGE(fmt,...) winlog(fmt  ,__VA_ARGS__)
	#elif LOTUS2D_PLATFORM ==LOTUS2D_PLATFORM_IOS
		void ioslog(const char* func,int line,const char * pszFormat, ...);	
		#define LOGV(fmt,...) ioslog(__PRETTY_FUNCTION__, __LINE__,fmt,  __VA_ARGS__)
		#define LOGD(fmt,...) ioslog(__PRETTY_FUNCTION__, __LINE__,fmt  ,__VA_ARGS__)
		#define LOGI(fmt,...) ioslog(__PRETTY_FUNCTION__, __LINE__,fmt  ,__VA_ARGS__)
		#define LOGW(fmt,...) ioslog(__PRETTY_FUNCTION__, __LINE__,fmt   ,__VA_ARGS__)
		#define LOGE(fmt,...) ioslog(__PRETTY_FUNCTION__, __LINE__,fmt  ,__VA_ARGS__)

	#endif
#else   
	#define LOGV(...)
	#define LOGD(...)
	#define LOGI(...)
	#define LOGW(...)
	#define LOGE(...)
#endif 

#ifdef __cplusplus
}
#endif

#endif
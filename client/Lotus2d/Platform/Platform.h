/********************************************************************
	created:	2012/09/11
	filename: 	Platform.h
	author:		reedhong
	
	purpose:	平台相关
*********************************************************************/

#ifndef __LOTUS2D_PLATFORM_H__
#define __LOTUS2D_PLATFORM_H__

#define LOTUS2D_PLATFORM_WIN32 1
#define LOTUS2D_PLATFORM_IOS 2
#define LOTUS2D_PLATFORM_ANDROID 3

#define LOTUS2D_ARCHITECTURE_64 1
#define LOTUS2D_ARCHITECTURE_32 2

/* Finds the current platform */
#if defined( __WIN32__ ) || defined( _WIN32 )
	#define LOTUS2D_PLATFORM LOTUS2D_PLATFORM_WIN32
#elif defined( __IOS__)
	#define LOTUS2D_PLATFORM LOTUS2D_PLATFORM_IOS
#elif defined(__ANDROID__)
#	define LOTUS2D_PLATFORM LOTUS2D_PLATFORM_ANDROID
#endif

	/* Find the arch type */
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#   define LOTUS2D_ARCH_TYPE LOTUS2D_ARCHITECTURE_64
#else
#   define LOTUS2D_ARCH_TYPE LOTUS2D_ARCHITECTURE_32
#endif


#endif
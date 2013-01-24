/********************************************************************
	created:	2012/09/20
	filename: 	AudioEngine.h
	author:		reedhong
	
	purpose:	声音引擎，接口的定义学习cocos2d-x
*********************************************************************/

#ifndef __Lotus2d_AudioEngine_H__
#define __Lotus2d_AudioEngine_H__

#include "Platform/Platform.h"

#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_WIN32
# include "Platform/win32/AudioEngineImp.h"
#elif LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_IOS
# include "Platform/iOS/AudioEngineImp.h"
#elif LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_ANDROID
# include "Platform/android/AudioEngineImp.h"
#endif


#endif

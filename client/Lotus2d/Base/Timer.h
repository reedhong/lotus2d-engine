/*
 * ²Ã¼õ×ÔOgre
 * created by reedhong 2012.07.18
 */
#ifndef __Lotus2d_Timer_H__
#define __Lotus2d_Timer_H__

#include "Platform/Platform.h"

//Bring in the specific platform's header file
#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_WIN32
# include "Platform/win32/TimerImp.h"
#elif LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_IOS
# include "Platform/ios/TimerImp.h"
#elif LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_ANDROID
# include "Platform/android/TimerImp.h"
#endif

#endif

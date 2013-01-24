/********************************************************************
	created:	2012/09/12
	filename: 	CameConfig.h
	author:		reedhong
	
	purpose:	GameConfig.h
*********************************************************************/

#ifndef __GameConfig_H__
#define __GameConfig_H__

#include "Platform/Platform.h"

#if LOTUS2D_PLATFORM ==LOTUS2D_PLATFORM_WIN32
#define RESOURCE_PATH "C:/github/lotus-game/client/resource/"
#elif LOTUS2D_PLATFORM ==LOTUS2D_PLATFORM_ANDROID
#define RESOURCE_PATH "/mnt/sdcard/lotus/"
#else
#define RESOURCE_PATH "/Users/reedhong/win7/github/lotus-game/resource/client/high/"
#endif


#define MAX_TOUCH_NUM	16	// 最多接受的事件

#define GAME_INTERVAL	60	// 每秒60帧

#define  SPEEDX_P			4//6;
#define  SPEEDY_P			4//6;
#define  SPEEDX_P_HERO		4//6;
#define  SPEEDY_P_HERO		2//3;

#define SPEED_BASE			2


#define LOGIN_SERVER_ADDRESS "192.168.0.1"
#define LOGIN_SERVER_PORT 8080


#define MAX_PACKET_LEN (32*1024)
#define PACKET_HADE_LEN 16

#define MAX_MD5PWD_LENGTH		(16)


#endif

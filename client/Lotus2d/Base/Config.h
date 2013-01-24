/********************************************************************
	created:	2012/09/11
	filename: 	Config.h
	author:		reedhong
	
	purpose:	涉及引擎全局性的信息和一些相关的配置， follow 《我的游戏编程感悟》
*********************************************************************/

#ifndef __Lotus2d_Config_H__
#define __Lotus2d_Config_H__


#define LOTUS2D_CONFIG_LITTLE_ENDIAN
#define LOTUS2D_PROFILING 1
#define ENABLE_GL_CHECK 1

// 打开内存分配调试和检测开关
#ifndef MEMORY_DEBUG 
	#define MEMORY_DEBUG 
#endif


#define LOTUS2D_RENDER_OPENGL 1
#define LOTUS2D_RENDER_OPENGLES1 2
#define LOTUS2D_RENDER_OPENGLES2 3


/* render config */
#if defined( __OPENGL__ )
#define LOTUS2D_RENDER LOTUS2D_RENDER_OPENGL
#elif defined( __OPENGLES1__)
#define LOTUS2D_RENDER LOTUS2D_RENDER_OPENGLES1
#elif defined(__OPENGLES2__)
#	define LOTUS2D_RENDER LOTUS2D_RENDER_OPENGLES2
#else
#	define LOTUS2D_RENDER LOTUS2D_RENDER_OPENGLES1
#endif


// Debug开关
#   if defined(_DEBUG) || defined(DEBUG) || defined(__DEBUG) || defined(__DEBUG__)
#       define LOTUS2D_DEBUG 
#  endif

// profile
#define Lotus2d_PROFILING 0

// 渲染器内部的顶点数组
#define MAX_ARRAY_SIZE 10*1000

// 字体
enum FONT_ID
{
	LITTLE_FONT = 14,
	MIDDLE_FONT =18,
	LARGE_FONT =20,
};

enum FONT_CACHE_IMAGE_SIZE
{
	FONT_CACHE_IMAGE_64x64,		///< 64x64
	FONT_CACHE_IMAGE_128x128,	///< 128x128
	FONT_CACHE_IMAGE_256x256,	///< 256x256
	FONT_CACHE_IMAGE_512x512, 	///< 512x512
	FONT_CACHE_IMAGE_1024x1024	///< 1024x1024
};

/* The padding between glyphs on the texture(in pixel). */
#define TEX_FONT_GLYPH_PADDING	6

// UString本地缓冲区的大小
#define LocalFixStringSize 127

// 网络缓冲区设置
//初始化的接收缓存长度
#define DEFAULTSOCKETINPUTBUFFERSIZE 64*1024
//最大可以允许的缓存长度，如果超过此数值，则断开连接
#define DISCONNECTSOCKETINPUTSIZE 96*1024

//初始化的发送缓存长度
#define DEFAULTSOCKETOUTPUTBUFFERSIZE 8192
//最大可以允许的缓存长度，如果超过此数值，则断开连接
#define DISCONNECTSOCKETOUTPUTSIZE 100*1024

#define USES_VBO 1

#define FRAME_COUNT_PER_SECOND 15 // 每秒帧数，在粒子中需要用到
#define FRAME_TIME (1.0f/FRAME_COUNT_PER_SECOND)


// ui control
#define INPUT_COLOR 0xFF006600
#define INPUT_TICK_COLOR 0xFFFF0000
#define INPUT_CONTENT_GAP 2 // 输入框中内容的头尾间隔
#define INPUT_DEFAULT_MAX_LEN 128 // 输入框默认字体的总数
#define INPUT_TICK_GAP 2	// 输入框中光标跟内容的间隔


// socket
#define DEFAULT_CONNECT_TIMEOUT 3 // 默认连接超时时间，3秒
#define DEFAULT_CONNECT_RETRY	3 // 默认重试连接次数

#endif
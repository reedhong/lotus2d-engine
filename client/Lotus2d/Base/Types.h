/********************************************************************
	created:	2012/09/14 
	filename: 	Types.h
	author:		reedhong 
	
	purpose:	类型声明
*********************************************************************/

#include "Platform/Platform.h"

#ifndef __Lotus2d_Types_H__
#define __Lotus2d_Types_H__

#include "Config.h"
#include "Platform/Platform.h"

#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;


#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_WIN32
#include <windows.h>
#endif

// todo
#ifndef BOOL
	#define BOOL int//signed char
#endif 

#define TRUE 1
#define FALSE 0

// Integer formats of fixed bit width
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef int int32;
typedef short int16;
typedef char int8;
typedef unsigned long long uint64;
typedef long long int64;
typedef unsigned short	PacketID_t;



struct FontAttribute
{
	int	size;		// 大小
	int	color;	// 颜色
	int	borderColor;	 // 如果字体在初始化时，border不为0才有意义，否则无用	
	int	align;	// 对齐
	bool isHorizontal; // 是否为水平方向
};


typedef union _Vector2f {
	struct { float x, y; } xy;
	struct { float u, v; } uv;
	float	arr[2];
} Vector2f;
//---------------------------------------------------------------------------------------------
typedef union _Vector3f {
	struct { float x, y, z; } xyz;
	struct { float r, g, b; } rgb;
	float	arr[3];
} Vector3f, Color3f;
//---------------------------------------------------------------------------------------------
typedef union _Vector4f {
	struct { float x, y, z, w; } xyzw;
	struct { float r, g, b, a; } rgba;
	struct { Vector3f abc; float d; } abcd;
	struct { Vector2f uv; float du, dv; } uvd;
	float	arr[4];

} Vector4f, Color4f;
//---------------------------------------------------------------------------------------------
typedef union _Vector2i {
	struct { int x, y; } xy;
	int		arr[2];
} Vector2i;
//---------------------------------------------------------------------------------------------
typedef union _Vector3i {
	struct { int x, y, z; } xyz;
	struct { int r, g, b; } rgb;
	int		arr[3];
} Vector3i, Color3i;
//---------------------------------------------------------------------------------------------
typedef union _Vector4i {
	struct { int x, y, z, w; } xyzw;
	struct { int r, g, b, a; } rgba;
	int		arr[4];
} Vector4i, Color4i;
//---------------------------------------------------------------------------------------------
typedef union _Vector2b {
	struct { unsigned char x, y; } xy;
	unsigned char	arr[2];
} Vector2b;
//---------------------------------------------------------------------------------------------
typedef union _Vector3b {
	struct { unsigned char x, y, z; } xyz;
	struct { unsigned char r, g, b; } rgb;
	unsigned char	arr[3];
} Vector3b, Color3b;
//---------------------------------------------------------------------------------------------
typedef union _Vector4b {
	struct { unsigned char x, y, z, w; } xyzw;
	struct { unsigned char r, g, b, a; } rgba;
	unsigned char	arr[4];
} Vector4b, Color4b;
//---------------------------------------------------------------------------------------------
typedef union _Matrix33f {
	struct { float m1, m2, m3,
		m4, m5, m6,
		m7, m8, m9; } individual;
	float	arr[9];
	float	m[3][3];
} Matrix33f;
//---------------------------------------------------------------------------------------------
typedef union _Matrix44f {
	struct { float m1, m2, m3, m4,
		m5, m6, m7, m8,
		m9, m10, m11, m12,
		m13, m14, m15, m16; } individual;
	struct { float rs[12]; Vector4f translation; } rstrans;
	float		arr[16];
	float		m[4][4];
	Vector4f	v[4];
} Matrix44f;
//---------------------------------------------------------------------------------------------
typedef union _Rect4i {
	struct { int left, right, top, bottom; } rect;
	int		arr[4];
} Rect4i;
//---------------------------------------------------------------------------------------------
typedef union _Rect6i {
	struct { int left, right, top, bottom,
		width, height; } rect;
	int		arr[6];
} Rect6i;
//---------------------------------------------------------------------------------------------
typedef union _Rect4f {
	struct { float xmin, xmax, ymin, ymax; } rect;
	float	arr[4];
} Rect4f;
//---------------------------------------------------------------------------------------------
typedef union _Rect6f {
	struct { float xmin, xmax, ymin, ymax,
		width, height; } rect;
	float	arr[6];
} Rect6f;
//---------------------------------------------------------------------------------------------
typedef union _Range2f {
	struct { float min, max; } range;
	float	arr[2];
} Range2f;
//---------------------------------------------------------------------------------------------
typedef union _Bound6f {
	struct { float xmin, xmax, ymin, ymax, zmin, zmax; } bound;
	struct { Range2f xrange, yrange, zrange; } range2f;
	Range2f	range[3];
	float	arr[6];
} Bound6f;
//---------------------------------------------------------------------------------------------
typedef union _Bound9f {
	struct { float xmin, xmax, ymin, ymax, zmin, zmax,
		width, height, length; } bound;
	struct { Bound6f box; float bounddwidth, boundheight, boundlength; } boundex;
	float	arr[9];
} Bound9f;
//---------------------------------------------------------------------------------------------


#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_WIN32
#define snprintf sprintf_s 
#endif 



#endif

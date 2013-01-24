/********************************************************************
	created:	2012/09/13
	filename: 	GLHeader.h
	author:		reedhong
	
	purpose:	
*********************************************************************/

#ifndef __Lotus2d_GLHeader_H__
#define __Lotus2d_GLHeader_H__

#include "Base/Config.h"

#if LOTUS2D_RENDER == LOTUS2D_RENDER_OPENGLES1 
	#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_IOS
	#import <OpenGLES/ES1/gl.h>
	#import <OpenGLES/ES1/glext.h>
	#else 
	#include "GLES/gl.h"
	#endif
#elif LOTUS2D_RENDER == LOTUS2D_RENDER_OPENGL
#include "GL/glew.h"
#endif

#endif
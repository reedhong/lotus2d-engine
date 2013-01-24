/********************************************************************
	created:	2012/09/19
	filename: 	Assert.h
	author:		reedhong
	
	purpose:	
*********************************************************************/

#ifndef _Lotus2d_ASSERT_H__
#define _Lotus2d_ASSERT_H__

#include "Config.h"
#include <assert.h>

#ifdef LOTUS2D_DEBUG
#ifndef ASSERT
#define ASSERT(c) if (c) 0; else assert(c)
#endif
#else
#define ASSERT(c) 0
#endif

#endif
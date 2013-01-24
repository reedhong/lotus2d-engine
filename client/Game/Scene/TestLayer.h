/********************************************************************
	created:	2012/10/23
	filename: 	TestLayer.h
	author:		reedhong
	
	purpose:	用于做各种测试
*********************************************************************/

#ifndef __TestLayer_H__
#define __TestLayer_H__

#include "AbstractLayer.h"
#include "Actor/Actor.h"
#include "RenderSystem/Sprite.h"
#include "RenderSystem/PatchSprite.h"
#include "RenderSystem/Texture.h"
#include "RenderSystem/Render.h"
using namespace Lotus2d;

class TestLayer: public AbstractLayer
{
public:
	TestLayer();
	virtual ~TestLayer();

	virtual void enter();

	virtual void leave();

	virtual void step();

	// todo:
	virtual int handleEvent(eActionEventType event, int x, int y);

	virtual void drawImp();

public:
	Texture*		m_texture;
	Actor*		m_actor;
	PatchSprite* m_patchSprite;
};

#endif

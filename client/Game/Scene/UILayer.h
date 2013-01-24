/********************************************************************
	created:	2012/10/23
	filename: 	TestLayer.h
	author:		reedhong
	
	purpose:	用于做各种测试
*********************************************************************/

#ifndef __UILayer_H__
#define __UILayer_H__

#include "UIEngine/Page.h"
using namespace Lotus2d;

#include "AbstractLayer.h"
#include "Generate/UI_COMMAND.h"

class UILayer: public AbstractLayer
{
public:
	UILayer();
	virtual ~UILayer();

	virtual void enter();

	virtual void leave();

	virtual void step();
	void setCurrentPage(unsigned int key);
	// todo:
	virtual int handleEvent(eActionEventType event, int x, int y);

	virtual void drawImp();
public:
	Page* m_currentPage;
};
#endif

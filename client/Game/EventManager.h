/********************************************************************
	created:	2012/10/22
	filename: 	EventManager.h
	author:		reedhong
	
	purpose:	事件管理
*********************************************************************/

#ifndef __EventManager_H__
#define __EventManager_H__

#include "Constants/GameConfig.h"

#include "Event/Event.h"
#include "Event/EventManagerBase.h"
using namespace  Lotus2d;


class EventManager: public EventManagerBase
{
protected:
	EventManager();
public:
	static EventManager* Instance();
	~EventManager();
	void addAction(eRawActionEventType event, int x, int y);
	virtual void handleEvent(eActionEventType event, int x, int y);
	void step();
};

#endif

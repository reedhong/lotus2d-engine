/********************************************************************
	created:	2012/10/22
	filename: 	EventManager.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/
#include "RenderSystem/Render.h"
using namespace Lotus2d;

#include "EventManager.h"
#include "Scene/Screen.h"
#include "Game.h"


EventManager::EventManager()
{
}

EventManager::~EventManager()
{

}

EventManager* EventManager::Instance()
{
	static EventManager instance;
	setInstance(&instance);
	return &instance;
}

void EventManager::addAction(eRawActionEventType event, int x, int y)
{
	float scale = Render::Instance()->getScale();
	RawActionEvent rae;
	rae.m_type = event;
	rae.m_posX = int(x/scale);
	rae.m_posY = int(y/scale);
	
	pushEvent(rae);
}


void EventManager::handleEvent(eActionEventType type, int x, int y)
{
	EventManagerBase::handleEvent(type, x, y);
	switch(type){
	case  eActionGoBack:
		Game::Instance()->gameEnd();
		break;
	default:
		Screen::Instance()->handleEvent(type, x, y);
		break;
	}
}

void EventManager::step()
{
	dealEvents();
}
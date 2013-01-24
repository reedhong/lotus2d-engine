/********************************************************************
	created:	2012/12/13
	filename: 	UICommandHander.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "UIManager.h"




UIManager::UIManager()
{

}

UIManager::~UIManager()
{

}

UIManager* UIManager::Instance()
{
	static UIManager s_instance;
	return &s_instance;
}

void UIManager::initHandlers()
{
	//m_handlers[UI_COMMAND::login::LOGIN] = UI_HANDLER::login::doLogin;
	//m_handlers[UI_COMMAND::main::HERO_PROPERTY] = UI_HANDLER::main::doHeroProperty;
}

void UIManager::s_handleCommandCallback(int command, Control* control)
{
	UIManager::Instance()->handleCommand(command, control);
}

void UIManager::registerHandler(int command, UICommandFunc func)
{
	map<int, UICommandFunc>::iterator iter = m_handlers.find(command);
	if( iter != m_handlers.end()){
		LOGD("command %d has register", command);
		ASSERT(0);
	}

	m_handlers[command]  = func;
}

void UIManager::handleCommand(int command, Control* control)
{
	UICommandFunc func = m_handlers[command];
	if(func){
		func(control);
	}else{
		ASSERT(0);
	}
}

/********************************************************************
	created:	2012/12/13
	filename: 	UICommandHander.h
	author:		reedhong
	
	purpose:	响应UI MainEvent
*********************************************************************/

#ifndef __UIManager_H__
#define __UIManager_H__

#include "Base/Prerequisites.h"
#include "UIEngine/Control.h"
using namespace Lotus2d;

typedef void (*UICommandFunc )(Control* control );

class UIManager
{
public:
	UIManager();
	~UIManager();
	static UIManager* Instance();
	void initHandlers();
	void registerHandler(int command, UICommandFunc func);
	void handleCommand(int command, Control* control);

	/*
	 * 响应UI事件接口, 设置到UIEngine中去
	 */
	static void s_handleCommandCallback(int command, Control* control);
private:
	map<int, UICommandFunc>		m_handlers;
};


#endif

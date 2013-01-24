/********************************************************************
	created:	2012/12/10
	filename: 	EventManagerBase.h
	author:		reedhong
	
	purpose:	
*********************************************************************/

#ifndef __Lotus2d_EventManagerBase_H__
#define __Lotus2d_EventManagerBase_H__

#include "Event.h"
#include "Base/Config.h"

namespace Lotus2d {

	// config
	
	#define MAX_EVENT	16
	#define	LONG_PRESS_JUDGE_DISTANCE	  10
	#define	LONG_PRESS_ACC_DISTANCE		  50
	#define	ACC_PRESS_DURATION			500  
	#define	HOLD_PRESS_DURATION			2000  
	#define	HOLD_DRAG_DURATION			50  
	#define	MAX_ACC_SPEED		30

	#define	MOVE_DIRECTION_ALL				0
	#define	MOVE_DIRECTION_HORIZONAL		1
	#define	MOVE_DIRECTION_VERTICAL		2

	class EventManagerBase
	{
	protected:
		EventManagerBase();
		virtual ~EventManagerBase();
	public:
		static EventManagerBase* getInstance();
		static void setInstance(EventManagerBase* emb);

		bool pushEvent(const RawActionEvent& event);
		bool popEvent(RawActionEvent& event);

		void dealEvents();

		virtual void handleEvent(eActionEventType event, int x, int y);
	protected:
		static EventManagerBase* s_instance;
	private:
		RawActionEvent		m_rawEvents[MAX_EVENT];

		// 如果一个基本变量被volatile修饰，编译器将不会把它保存到寄存器中，  
		//而是每一次都去访问内存中实际保存该变量的位置上。这一点就避免了  
		//没有volatile修饰的变量在多线程的读写中所产生的由于编译器优化所导致的灾难性问题。 
		volatile  unsigned int 	m_pushCount;
		volatile  unsigned int 	m_popCount;

	};

}
#endif
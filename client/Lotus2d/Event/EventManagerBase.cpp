/********************************************************************
	created:	2012/12/10
	filename: 	EventManagerBase.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "EventManagerBase.h"
#include "Base/Prerequisites.h"

namespace Lotus2d {
	EventManagerBase* EventManagerBase::s_instance = NULL;

	EventManagerBase::EventManagerBase()
	{
		m_pushCount = 0;
		m_popCount = 0;
	}
	
	EventManagerBase::~EventManagerBase()
	{

	}
		
	EventManagerBase* EventManagerBase::getInstance()
	{
		return s_instance;
	}

	void EventManagerBase::setInstance(EventManagerBase* emb)
	{
		s_instance = emb;
	}

	bool EventManagerBase::pushEvent(const RawActionEvent& event)
	{
		if (m_pushCount - m_popCount < MAX_EVENT)     {     
			m_rawEvents[m_pushCount & MAX_EVENT] = event;     
			m_pushCount++;     
			return true;     
		}     
		
		return false;  
	}

	bool EventManagerBase::popEvent(RawActionEvent& event)
	{
		// ÅÐ¶ÏÊÇ·ñÎª¿Õ     
		if (m_pushCount != m_popCount)     {     
			event = m_rawEvents[m_popCount & MAX_EVENT];
			m_popCount++;  
			return true;   
		}     
  
		return false; 
	}


	void EventManagerBase::dealEvents()
	{
		RawActionEvent evt;
		while( popEvent(evt)){
			switch(evt.m_type){
			case eRawActionDown:
				handleEvent(eActionDown, evt.m_posX, evt.m_posY);
				break;
			case eRawActionUp:
				handleEvent(eActionUp, evt.m_posX, evt.m_posY);
				break;
			case eRawActionMove:
				break;
			case eRawActionGoBack:
				break;
			case eRawActionPinchScaleUp:
				break;
			case eRawActionPinchScaleDown:
				break;
			default:
				break;
			}
		}
	}

	void EventManagerBase::handleEvent(eActionEventType event, int x, int y)
	{

	}
}

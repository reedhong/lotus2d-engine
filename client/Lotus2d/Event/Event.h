/********************************************************************
	created:	2012/10/22
	filename: 	Event.h
	author:		reedhong
	
	purpose:	定义事件
*********************************************************************/

#ifndef __Lotus2d_Event_H__
#define __Lotus2d_Event_H__

namespace Lotus2d {

	// 平台原始的事件
	enum eRawActionEventType{
		eRawActionDown = 0,
		eRawActionUp,
		eRawActionMove,
		eRawActionGoBack,
		eRawActionPinchScaleUp,
		eRawActionPinchScaleDown,
		eRawActionNull,
	};

	class RawActionEvent
	{
	public:
	
		RawActionEvent(eRawActionEventType type=eRawActionNull, unsigned long time = 0, int x=0, int y=0):m_type(type), m_time(time), m_posX(x), m_posY(y)
		{

		}
		RawActionEvent(const RawActionEvent& evt)
		{
			m_type = evt.m_type;
			m_time = evt.m_time;
			m_posX = evt.m_posX;
			m_posY = evt.m_posY;
		}

		RawActionEvent& operator = (const RawActionEvent& evt)
		{
			m_type = evt.m_type;
			m_time = evt.m_time;
			m_posX = evt.m_posX;
			m_posY = evt.m_posY;
			return *this;
		}
	public:
		eRawActionEventType	m_type;
		int										m_posX;
		int										m_posY;
		unsigned long 					m_time;
	};

	// 应用层事件
	enum eActionEventType{
		eActionDown = 0,
		eActionUp,
		eActionMove,
		eActionHold,
		eActionHoldUp,
		eActionLongMoveUp,
		eActionZoomIn,
		eActionZoomOut,
		eActionCancel,
		eActionGoBack,
		eActionAccelerate,
		eActionNull
	};
}

#endif
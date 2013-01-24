/********************************************************************
	created:	2013/01/08
	filename: 	Tab.h
	author:		reedhong
	
	purpose:	
*********************************************************************/
#ifndef __Lotus2d_Tab_H__
#define __Lotus2d_Tab_H__

#include "Panel.h"

namespace Lotus2d {
	class Tab: public Panel
	{
	public:
		Tab();
		virtual ~Tab();
		virtual Tab* copy();

		/* 
		 * 从stream中加载控件信息
		 */
		virtual void loadImp(Stream* stream) ;
		/*
		 * 绘制控件
		 */
		virtual void drawImp(int offsetX = 0, int offsetY = 0);

		virtual bool handleEventImp(eActionEventType event, int posX, int posY, int offsetX=0,int offsetY=0);
		void setSelectedItem(int index);

		virtual void addChild(Control* control);
	public:
		// load from binary
		int	m_childPageX;
		int	m_childPageY;
		
		int	m_selectedIndex;
	};
}

#endif // __Tab_H__
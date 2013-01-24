/********************************************************************
	created:	2012/09/22
	filename: 	Page.h
	author:		reedhong
	
	purpose:	 由UI控件组成的一个完成UI
*********************************************************************/

#ifndef __Lotus2d_Page_H__
#define __Lotus2d_Page_H__

#include "Panel.h"

namespace Lotus2d {
	class Page: public Panel
	{
	public:
		Page();
		~Page();
		static Control* loadControlByType(Stream* stream);
		virtual bool hasChild() { return true;}
		virtual void loadImp(Stream* stream);
		virtual void drawImp(int posX = 0, int posY = 0);
		virtual bool handleEventImp(eActionEventType event, int posX, int posY, int offsetX=0,int offsetY=0);
	public:
		int		m_key;
		int		m_offsetX;
		int		m_offsetY;
	};
}


#endif
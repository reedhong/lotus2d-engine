/********************************************************************
	created:	2013/01/06
	filename: 	List.h
	author:		reedhong
	
	purpose:	
*********************************************************************/

#ifndef __Lotus2d_List_H__
#define __Lotus2d_List_H__

#include "Control.h"
#include "Panel.h"

namespace Lotus2d {
	class List: public Control
	{
	public:
		List();
		virtual ~List();
		virtual List* copy();

		/* 
		 * 从stream中加载控件信息
		 */
		virtual void loadImp(Stream* stream) ;
		/*
		 * 绘制控件
		 */
		virtual void drawImp(int offsetX = 0, int offsetY = 0);

		virtual bool handleEventImp(eActionEventType event, int posX, int posY, int offsetX=0,int offsetY=0);

		virtual bool hasChild() { return true;}
		virtual void addChild(Control* control);
		virtual Control* getFirstChild(){ return m_item;}
		virtual Control* getNextChild() { return NULL;}
		virtual void deleteChild(Control* control);

		void setSize(unsigned int size);
		void setItemInfo();
	private:
		void addOneItem();
		void clearItems();
		
	public:

		// load from binary
		Panel*						m_item;
		unsigned char		m_trend;
		unsigned char		m_slideMode;
		int							m_marginWidth;
		int							m_marginHeight;

		// self
		unsigned int			m_size;
		int							m_startOffset;
		int							m_itemWidth;
		int							m_itemHeight;
		vector<Panel *>	m_items;

		int							m_pageSize;
		int							m_totalPage;
		int							m_pageWidth;
	};
}


#endif // __List_H__
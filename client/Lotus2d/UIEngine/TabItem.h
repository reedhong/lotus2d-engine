/********************************************************************
	created:	2013/01/08
	filename: 	TabItem.h
	author:		reedhong
	
	purpose:	
*********************************************************************/
#ifndef __Lotus2d_TabItem_H__
#define __Lotus2d_TabItem_H__

#include "Text.h"
#include "Page.h"

namespace Lotus2d {
	class TabItem: public Text
	{
	public:
		TabItem();
		virtual ~TabItem();
		virtual TabItem* copy();

		/* 
		 * 从stream中加载控件信息
		 */
		virtual void loadImp(Stream* stream) ;
		/*
		 * 绘制控件
		 */
		virtual void drawImp(int offsetX = 0, int offsetY = 0);

		virtual bool handleEventImp(eActionEventType event, int posX, int posY, int offsetX=0,int offsetY=0);
		Page* getChildPage() { return m_childPage;}
	protected:
		Page*		m_childPage;
		int			m_pageId;
	};
}

#endif // __TabItem_H__
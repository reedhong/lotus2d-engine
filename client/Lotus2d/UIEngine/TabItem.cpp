/********************************************************************
	created:	2013/01/08
	filename: 	TabItem.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "TabItem.h"
#include "Page.h"
#include "ControlDef.h"

namespace Lotus2d {
	TabItem::TabItem()
	{
		m_type = eCtrlTabItem;
		m_childPage = NULL;
		m_focusable = true;
	}
	
	TabItem::~TabItem()
	{

	}
	
	TabItem* TabItem::copy()
	{
		return NULL;
	}

	void TabItem::loadImp(Stream* stream)
	{
		Text::loadImp(stream);
		m_pageId = stream->read16le();
		// todo:

	}

	void TabItem::drawImp(int offsetX, int offsetY)
	{
		if(m_isFocused){ // ÇÐ»»

		}
		Text::draw(m_x + offsetX, m_y+offsetY);
	}
	
	bool TabItem::handleEventImp(eActionEventType event, int posX, int posY, int offsetX,int offsetY)
	{
		return false;
	}
}
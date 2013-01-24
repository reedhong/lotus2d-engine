/********************************************************************
	created:	2013/01/08
	filename: 	Tab.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "UIEngine/ControlAll.h"

namespace Lotus2d {
	Tab::Tab()
	{
		m_type = eCtrlTab;
		m_selectedIndex = 0;
	}
	
	Tab::~Tab()
	{

	}
	
	Tab* Tab::copy()
	{
		return NULL;
	}

	void Tab::loadImp(Stream* stream)
	{
		Panel::loadImp(stream);
	}

	void Tab::drawImp(int posX, int posY)
	{
		if( 0<= m_selectedIndex && m_selectedIndex < (int)m_childControls.size()){
			TabItem* control = (TabItem *)m_childControls[m_selectedIndex];
			Page* childPage = control->getChildPage();
			if(childPage){
				childPage->draw(m_x+posX+m_childPageX, m_y+posY+m_childPageY);
			}
		}
		// itemºó»­
		Panel::drawImp(posX, posY);
	}
	
	bool Tab::handleEventImp(eActionEventType event, int posX, int posY, int offsetX,int offsetY)
	{
		return Panel::handleEventImp(event, posX, posY, offsetX,offsetY);
	}

	void Tab::setSelectedItem(int index)
	{
		if(0<=index && index < (int)m_childControls.size()){
			m_selectedIndex = index; 
		}
		// dispatchEvent  
		// todo:
	}

	void Tab::addChild(Control* control)
	{
		ASSERT( control->m_type== eCtrlTabItem);
		Panel::addChild(control);
	}
}
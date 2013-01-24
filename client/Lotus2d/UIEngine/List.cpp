/********************************************************************
	created:	2013/01/06
	filename: 	List.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "List.h"
#include "ControlDef.h"
#include "RenderSystem/Render.h"

namespace Lotus2d {

	List::List()
	{
		m_type = eCtrlList;
		m_item = NULL;
		m_size = 0;
		m_trend = eTrendVertical;
		m_slideMode = eSlideNormal;
		m_startOffset = 0;
		m_pageSize = 0;
		m_pageWidth = 0;
		m_marginHeight = 0;
		m_marginWidth = 0;
	}

	List::~List()
	{
		clearItems();
	}

	List* List::copy()
	{
		ASSERT(0);
		return NULL;
	}

	void List::loadImp(Stream* stream) 
	{	
		m_item = new Panel();
		m_item->load(stream);

		m_trend = stream->read8();
		m_slideMode = stream->read8();
		m_marginWidth =  (int)(stream->read16le() * GET_SCALE_FACTOR());
		m_marginHeight =  (int)(stream->read16le() * GET_SCALE_FACTOR());

		setItemInfo();
	}

	void List::drawImp(int offsetX, int offsetY)
	{
		Render::Instance()->setClip(offsetX+m_x, offsetY+m_y, m_width, m_height);
		for(int i = 0; i < (int)m_size; i++){
			Panel* panel = m_items[i];
			if(m_trend == eTrendHorizon){	// Ë®Æ½ 
				if ((i * (m_itemWidth + m_marginWidth)) + m_startOffset < (m_width + m_itemWidth) && ((i * (m_itemWidth + m_marginWidth)) + m_startOffset) >(-m_itemWidth)){
					panel->draw(m_x + (i * (m_itemWidth + m_marginWidth)) + offsetX + m_startOffset + m_marginWidth, 
						m_y + offsetY + m_marginHeight);
				}
			}else if(m_trend == eTrendVertical){	// ´¹Ö±
				if ((i * (m_itemHeight + m_marginHeight)) + m_startOffset < (m_height + m_itemHeight)  && (i * (m_itemHeight + m_marginHeight)) + m_startOffset >(-m_itemHeight)){
					panel->draw(m_x + offsetX + m_marginWidth, m_y + m_marginHeight +  (i * (m_itemHeight + m_marginHeight)) + offsetY + m_startOffset + m_marginHeight);
				}
			}
		}
		Render::Instance()->disableClip();
	}

	bool List::handleEventImp(eActionEventType event, int posX, int posY, int offsetX,int offsetY)
	{
		
		return false;
	}

	void List::setSize(unsigned int size)
	{
		if (!m_item || size == m_size){
			return;
		}
		if( size < m_size){
			for(unsigned int i = 0; i < m_size-size; i++){
				Panel* tmp = m_items[size+i];
				SAFE_DELETE(tmp);
			}
			m_items.erase(m_items.begin()+size, m_items.end());
		}else{
			for(unsigned int i = 0; i < size-m_size; i++){
				addOneItem();
			}
		}
		m_size = size;
		// todo:
		//SetTotalWH(size);
	}

	void List::addOneItem()
	{
		Panel* item = m_item->copy();
		m_items.push_back(item);
	}

	void List::clearItems()
	{
		SAFE_DELETE(m_item);
		vector<Panel *>::iterator iter = m_items.begin();
		for(; iter != m_items.end(); ++iter){
			SAFE_DELETE(*iter);
		}
		m_items.clear();
		m_size = 0;
	}
	void List::addChild(Control* control)
	{
		ASSERT( control->m_type== eCtrlPanel);
		SAFE_DELETE(m_item);
		clearItems();
		m_item = (Panel* )control;
		setSize(1);
		setItemInfo();
		m_item->m_parent = this;
	}

	void List::deleteChild(Control* control)
	{
		ASSERT( control->m_type== eCtrlPanel);
		SAFE_DELETE(m_item);
		clearItems();
		m_item = NULL;
	}
	void List::setItemInfo()
	{
		if(!m_item) return ;

		m_itemWidth  = m_item->m_width;
		m_itemHeight = m_item->m_width;
		m_item->m_x = 0;
		m_item->m_y = 0;

		if (m_slideMode == eSlidePage){
			if(m_trend == eTrendHorizon){
				m_pageSize = m_width / (m_itemWidth + m_marginWidth);
				m_pageWidth = m_pageSize * (m_itemWidth + m_marginWidth);
				m_width = m_pageWidth;
			}else if(m_trend == eTrendVertical){
				m_pageSize = m_height / (m_itemHeight + m_marginHeight);
				m_pageWidth = m_pageSize * (m_itemHeight + m_marginHeight);
				m_height = m_pageWidth;
			}
		}
	}

}
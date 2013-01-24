/********************************************************************
	created:	2012/11/08
	filename: 	Panel.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "Panel.h"
#include "ControlDef.h"
#include "Page.h"

#include "RenderSystem/Render.h"


namespace Lotus2d {
	Panel::Panel()
	{
		m_type = eCtrlPanel;	
		m_focusable = true;
	}

	Panel::~Panel()
	{
		vector<Control* >::iterator iter = m_childControls.begin();
		for(; iter != m_childControls.end(); ++iter){
			delete (*iter);
		}
		m_childControls.clear();
	}

	Panel* Panel::copy()
	{
		Panel* clone = new Panel(); 
		Control::copy(clone);

		Control* temp = NULL;
		Control* copycontrol = NULL;
		int size = (int)m_childControls.size();
		for (int i=0;i<size;i++){
			temp = m_childControls[i];
			copycontrol = temp->copy();
			clone->m_childControls.push_back(copycontrol);
		}
		return clone;
	}

	void Panel::loadImp(Stream* stream) 
	{
		int8 size = stream->read8();
		Control* control = NULL;
		for (int32 i=0;i<size;i++)
		{
			control = Page::loadControlByType(stream);
			ASSERT(control!=NULL);
			m_childControls.push_back(control);
		}

		// todo: 
		m_focusable = true; 
	}

	void Panel::drawImp(int posX, int posY)
	{
		vector<Control* >::iterator iter = m_childControls.begin();
		for(; iter != m_childControls.end(); ++iter){
			(*iter)->draw(m_x+posX, m_y+posY);
		}
	}

	bool Panel::handleEventImp(eActionEventType event, int posX, int posY, int offsetX,int offsetY)
	{
		vector<Control* >::reverse_iterator  iter = m_childControls.rbegin();
		for(; iter != m_childControls.rend();  ++iter){
			bool bRet = (*iter)->handleActionEvent(event, posX, posY, m_x+offsetX, m_y+offsetY);
			if(bRet)
				return true;
		}
		return false;
	}


	void Panel::addChild(Control* control)
	{
		m_childControls.push_back(control);
		control->m_parent = this;
	}

	void Panel::deleteChild(Control* control)
	{
		vector<Control* >::iterator iter = m_childControls.begin();
		for(; iter != m_childControls.end(); ++iter){
			if( control == (*iter)){
				m_childControls.erase(iter);
				break;
			}
		}
	}

	Control* Panel::getFirstChild()
	{
		m_iter = m_childControls.begin();
		if(m_iter != m_childControls.end()){
			return *m_iter;
		}else{
			return NULL;
		}
	}

	Control* Panel::getNextChild()
	{
		m_iter++;
		if(m_iter != m_childControls.end()){
			return *m_iter;
		}else{
			return NULL;
		}
	}

	void Panel::forwardControl(Control* control, bool isup)
	{
		vector<Control* >::iterator iter = m_childControls.begin();
		for(; iter != m_childControls.end(); ++iter){
			if( control == (*iter)){
				break;
			}
		}
		if( iter == m_childControls.end())
			return ;
		vector<Control* >::iterator iter_swap;
		if(isup){
			if(iter == m_childControls.begin()){
				iter_swap = m_childControls.end()-1;
			}else{
				iter_swap = (iter-1);
			}
		}else{
			if( (iter+1) == (m_childControls.end())){
				iter_swap = m_childControls.begin();
			}else{
				iter_swap = (iter+1);
			}
		}
		swap(*iter, *iter_swap);
	}
}

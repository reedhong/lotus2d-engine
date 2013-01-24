/********************************************************************
	created:	2012/09/22
	filename: 	Page.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "Page.h"
#include "ControlAll.h"

namespace Lotus2d {
	Page::Page()
	{
		m_type = eCtrlPage;
		m_focusable = true;
		m_offsetX = 0;
		m_offsetY = 0;
	}

	Page::~Page()
	{
		
	}

	Control* Page::loadControlByType(Stream* stream)
	{
		unsigned char type = stream->read8();
		switch(type){
		case eCtrlText:
			{
				Text* control = new Text();
				control->load(stream);
				return control;
			}
		case eCtrlInput:
			{
				Input* control = new Input();
				control->load(stream);
				return control;
			}
		case eCtrlAnimation:
			{
				AnimationCtrl* control = new AnimationCtrl();
				control->load(stream);
				return control;
			}
		case eCtrlImage:
			{
				ImageCtrl* control = new ImageCtrl();
				control->load(stream);
				return control;
			}
		case eCtrlPanel:
			{
				Panel* control = new Panel();
				control->load(stream);
				return control;
			}
		case eCtrlPatch:
			{
				PatchCtrl* control = new PatchCtrl();
				control->load(stream);
				return control;
			}
		}

		return NULL;
	}

	void Page::loadImp(Stream* stream)
	{
		m_type = stream->read8();
		int size = (int)stream->read8();
		for (int i=0;i<size;i++){
			Control* control = Page::loadControlByType(stream);
			ASSERT(control!=NULL);
			m_childControls.push_back(control);
		}
	}

	void Page::drawImp(int posX, int posY)
	{
		Panel::drawImp(posX, posY);
	}

	bool Page::handleEventImp(eActionEventType event, int posX, int posY, int offsetX,int offsetY)
	{
		return Panel::handleEventImp(event, posX, posY, offsetX,offsetY);
	}

}
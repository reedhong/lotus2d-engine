/********************************************************************
	created:	2012/11/09
	filename: 	PatchCtrl.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "PatchCtrl.h"
#include "Resource/ResManagerBase.h"

namespace Lotus2d {

	PatchCtrl::PatchCtrl()
	{
		m_type = eCtrlPatch;
		m_patchSpriteKey = 0;
		m_index = 0;	
		m_patchSprite = NULL;
	}

	PatchCtrl::~PatchCtrl()
	{
		SAFE_RELEASE(m_patchSprite);
	}

	PatchCtrl* PatchCtrl::copy()
	{
		PatchCtrl* clone = new PatchCtrl();
		Control::copy(clone);

		clone->m_patchSpriteKey = m_patchSpriteKey;
		clone->m_index = m_index;	

		clone->setPatchSprite(m_patchSprite);
		
		return clone;
	}


	void PatchCtrl::loadImp(Stream* stream)
	{
		m_patchSpriteKey = (int)stream->read32le();
		m_index= (int)stream->read16le();
		setPatchSprite((PatchSprite *)ResourceManagerBase::getInstance()->loadResouce(m_patchSpriteKey, eResPatchSprite));
	}

	void PatchCtrl::drawImp(int offsetX, int offsetY)
	{
		if(m_index >= 0 && m_patchSprite){
			m_patchSprite->drawPatch(m_index, m_x+offsetX, m_y+offsetY, m_width, m_height, true);
		}
	}

	bool PatchCtrl::handleEventImp(eActionEventType event, int posX, int posY, int offsetX,int offsetY)
	{
		return false;
	}

	void PatchCtrl::setPatchSprite(PatchSprite* sprite)
	{
		SAFE_RELEASE(m_patchSprite);
		m_patchSprite = sprite;
		SAFE_ADDREF(m_patchSprite);
	}

}
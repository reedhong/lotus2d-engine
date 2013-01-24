/********************************************************************
	created:	2012/11/08
	filename: 	AnimationCtrl.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "AnimationCtrl.h"
#include "ControlDef.h"

#include "RenderSystem/Render.h"
#include "Resource/ResManagerBase.h"

namespace Lotus2d {

	AnimationCtrl::AnimationCtrl()
	{
		m_type = eCtrlAnimation;
		m_spriteKey = 0;
		m_action = 0;
		m_frameIndex = 0;
		m_frameDuration = 0;
		m_sprite = NULL;
		m_disable = false;
	}
		
	AnimationCtrl::~AnimationCtrl()
	{
		SAFE_RELEASE(m_sprite);
	}

	AnimationCtrl* AnimationCtrl::copy()
	{
		AnimationCtrl* clone = new AnimationCtrl();
		Control::copy(clone);

		clone->m_spriteKey = m_spriteKey;
		clone->m_action = m_action;
		clone->m_frameIndex = m_frameIndex;
		clone->m_frameDuration = m_frameDuration;
		clone->m_disable = m_disable;
		
		// 增加引用
		clone->setSprite(m_sprite);

		return clone;
	}

	void AnimationCtrl::loadImp(Stream* stream) 
	{
		m_spriteKey = (int)stream->read32le();
		m_action= (int)stream->read16le();
		m_disable =  (stream->read8()==1);

		if(m_spriteKey > 0){	
			setSprite((Sprite *)ResourceManagerBase::getInstance()->loadResouce(m_spriteKey, eResSprite));
		}

		// todo:
		m_mainEvent = 100;
		m_focusable = true;
	}

	void AnimationCtrl::drawImp(int offsetX, int offsetY)
	{
		if(!m_sprite) return ;
		int posX = m_x+offsetX+(m_width>>1);
		int posY = m_y +offsetY+(m_height>>1);
		if(m_disable){
			Render::Instance()->setTexEnv(eTEX_ENV_MODULATE);
			Render::Instance()->setColor(0.2f, 0.3f, 0.3f, 0.9f);
			m_sprite->drawAction(m_action, m_frameIndex,posX, posY); 
			Render::Instance()->end();
			Render::Instance()->setTexEnv(eTEX_ENV_REPLACE);
		}else{
			m_sprite->drawAction(m_action, m_frameIndex,posX, posY); 
		}
		gotoNextFrame();
	}

	bool AnimationCtrl::handleEventImp(eActionEventType event, int posX, int posY, int offsetX,int offsetY)
	{
		return false;
	}

	void AnimationCtrl::setSprite(Sprite* sprite)
	{
		SAFE_RELEASE(m_sprite);
		m_sprite = sprite;
		SAFE_ADDREF(m_sprite);
	}

	void AnimationCtrl::gotoNextFrame()
	{
		if(!m_sprite) return ;

		m_frameDuration++;
		SpriteAction* action = m_sprite->m_actions[m_action];
		SpriteAFrame* aframe = action->m_aframes[m_frameIndex];
		if(aframe->m_times == m_frameDuration){ // frame绘制结束
			m_frameIndex++;
			m_frameIndex%=action->m_aframes.size();
			m_frameDuration = 0;
		}
	}

}
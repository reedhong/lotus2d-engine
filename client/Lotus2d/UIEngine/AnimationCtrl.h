/********************************************************************
	created:	2012/11/08
	filename: 	AnimationCtrl.h
	author:		reedhong
	
	purpose:	
*********************************************************************/

#ifndef __Lotus2d_AnimationCtrl_H__
#define __Lotus2d_AnimationCtrl_H__

#include "Control.h"
#include "RenderSystem/Sprite.h"

namespace Lotus2d {
	class AnimationCtrl: public Control
	{
	public:
		AnimationCtrl();
		virtual ~AnimationCtrl();
		virtual AnimationCtrl* copy();

		/* 
		 * 从stream中加载控件信息
		 */
		virtual void loadImp(Stream* stream) ;
		/*
		 * 绘制控件
		 */
		virtual void drawImp(int offsetX = 0, int offsetY = 0);

		virtual bool handleEventImp(eActionEventType event, int posX, int posY, int offsetX=0,int offsetY=0);
		virtual bool hasChild() { return false;}
	
		void setSprite(Sprite* sprite);
	protected:
		void gotoNextFrame();
	public:
		int	m_spriteKey;
		int	m_action;	
		bool m_disable;		// 灰色
		int	m_frameIndex;
		int	m_frameDuration;
		
	private:
		Sprite*	m_sprite;


	};
}

#endif

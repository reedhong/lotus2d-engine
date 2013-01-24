/********************************************************************
	created:	2012/11/09
	filename: 	PatchCtrl.h
	author:		reedhong
	
	purpose:	背景一般用九宫格之类的方式去绘制，可以根据大小进行动态适应
*********************************************************************/

#ifndef __Lotus2d_PatchCtrl_H__
#define __Lotus2d_PatchCtrl_H__

#include "Control.h"
#include "ControlDef.h"
#include "RenderSystem/PatchSprite.h"

namespace Lotus2d {

	class PatchCtrl: public Control
	{
	public:
		PatchCtrl();
		virtual ~PatchCtrl();
		virtual PatchCtrl* copy();
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
		void setPatchSprite(PatchSprite* sprite);
	public:
		int	m_patchSpriteKey;
		int	m_index;	
	private:
		PatchSprite*	m_patchSprite;
	};
}


#endif
/********************************************************************
	created:	2012/11/08
	filename: 	Text.h
	author:		reedhong
	
	purpose:	文本
*********************************************************************/

#ifndef __Lotus2d_Text_H__
#define __Lotus2d_Text_H__

#include "Control.h"
#include "Font/FontManager.h"

namespace Lotus2d {

	class Text : public Control
	{
	public:
		Text();
		virtual ~Text();
		virtual Text* copy();
		Text* copy(Text* clone);
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
		void setText(const UString& content);
	public:
		UString				m_content;	
		FontAttribute	m_fontAttribute;
	};
}


#endif
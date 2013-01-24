/********************************************************************
	created:	2012/11/08
	filename: 	Image.h
	author:		reedhong
	
	purpose:	大部分能够用AnimationCtrl搞定，但在搞验证码的时候这个很需要
*********************************************************************/

#ifndef __Lotsu2d_Image_H__
#define __Lotsu2d_Image_H__

#include "Control.h"
#include "RenderSystem/Image.h"

namespace Lotus2d {

	class ImageCtrl: public Control
	{
	public:
		ImageCtrl();
		virtual ~ImageCtrl();
		virtual ImageCtrl* copy();
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
		void setImage(const Image& image);
	protected:
		Image 	m_image;

	};

}


#endif

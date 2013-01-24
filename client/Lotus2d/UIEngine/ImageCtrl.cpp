/********************************************************************
	created:	2012/11/08
	filename: 	Image.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "ImageCtrl.h"
#include "ControlDef.h"

#include "RenderSystem/Render.h"

namespace Lotus2d {

	ImageCtrl::ImageCtrl()
	{
		m_type = eCtrlImage;
	}

	ImageCtrl::~ImageCtrl()
	{
	}

	ImageCtrl* ImageCtrl::copy()
	{
		ImageCtrl* clone = new ImageCtrl();
		Control::copy(clone);

		clone->m_image  = m_image;

		return clone;
	}

	void ImageCtrl::loadImp(Stream* stream)
	{
			// nothing
	}

	void ImageCtrl::drawImp(int offsetX, int offsetY)
	{
		if(m_image.isRendable()){
			Render::Instance()->drawImage(&m_image, (float)(m_x+offsetX), (float)(m_y+offsetY));	
		}
	}

	bool ImageCtrl::handleEventImp(eActionEventType event, int posX, int posY, int offsetX,int offsetY)
	{
		return false;
	}

	void ImageCtrl::setImage(const Image& image)
	{
		m_image = image;
	}

}
/********************************************************************
	created:	2012/09/11
	filename: 	Image.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "Base/Macros.h"
#include "Image.h"


namespace Lotus2d{
	Image::Image():m_texture(NULL)
	{
		
	}

	 Image::Image(Texture* texturePtr, int  x, int  y, int  w,  int  h, BOOL flipY)
	{
		m_texture = texturePtr;
		if(m_texture){
			m_texture->addRef();
		}
		setTextureRect(x, y, w, h, flipY);
	}

	 Image::Image(const Image & image)
	{
		setTexture(image.m_texture);
		m_tx0 = image.m_tx0;
		m_ty0 = image.m_ty0;
		m_tx1= image.m_tx1;
		m_ty1 =image.m_ty1;

		m_x = image.m_x;
		m_y = image.m_y;
		m_width = image.m_width;
		m_height = image.m_height;

	}

	 Image&  Image::operator=(const Image& image)
	{
		if( this == &image)
			return *this;

		setTexture(image.m_texture);
		m_tx0 = image.m_tx0;
		m_ty0 = image.m_ty0;
		m_tx1= image.m_tx1;
		m_ty1 =image.m_ty1;

		m_x = image.m_x;
		m_y = image.m_y;
		m_width = image.m_width;
		m_height = image.m_height;

		return *this;
	}

	Image::~Image()
	{
		SAFE_RELEASE(m_texture);
	}

	bool Image::isRendable() const
	{
		return (m_texture) && (m_texture->isRendable());
	}



	void   Image::setTexture(Texture* texturePtr)
	{
		SAFE_RELEASE(m_texture);
		m_texture = texturePtr;
		SAFE_ADDREF(m_texture);
	}
	
	void Image::setTextureRect(int  x, int  y, int  w,  int  h, BOOL flipY)
	{ 

		m_x = (float)x;
		m_y = (float)y;
		m_width = (float)w;
		m_height = (float)h;

		m_tx0 = m_x/m_texture->m_textureWidth;
		m_tx1 = (m_x+m_width)/m_texture->m_textureWidth;

		if(flipY){
			m_ty0 = 1.0f-m_y/m_texture->m_textureHeight;
			m_ty1 = 1.0f-(m_y+m_height)/m_texture->m_textureHeight;
		}else{
			this->m_ty0 = m_y/m_texture->m_textureHeight;
			this->m_ty1 = (m_y+m_height)/m_texture->m_textureHeight;
		}
	}
}
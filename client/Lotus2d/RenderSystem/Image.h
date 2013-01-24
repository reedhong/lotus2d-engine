/********************************************************************
	created:	2012/09/11
	filename: 	Image.h
	author:		reedhong
	
	purpose:	
*********************************************************************/

#ifndef __Lotus2d_Image_H__
#define __Lotus2d_Image_H__

#include "Texture.h"
#include "Base/ObjectBase.h"

namespace Lotus2d {
	class Image:public ObjectBase
	{
	public:
		Image();
		Image(Texture* texturePtr, int  x, int  y, int  w,  int  h, BOOL flipY=FALSE);
		Image(const Image & image);
		~Image();
		Image& operator=(const Image& image);
		bool isRendable() const;
	public:
		void setTexture(Texture* texturePtr);
		void setTextureRect(int  x, int  y, int  w,  int  h, BOOL flipY=FALSE);
	public:
		Texture*	m_texture;	

		float m_tx0;
		float m_ty0;
		float m_tx1;
		float m_ty1;

		float m_x;
		float m_y;
		float m_width;
		float m_height;

	};
}

#endif

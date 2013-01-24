/********************************************************************
	created:	2012/09/13
	filename: 	Texture.h
	author:		reedhong
	
	purpose:	材质
*********************************************************************/

#ifndef __Lotus_Texture_H__
#define __Lotus_Texture_H__

#include "Base/ObjectBase.h"
#include "Base/Stream.h"
#include "Resource/Resource.h"

namespace Lotus2d {

	class Texture:public Resource
	{
	protected:
		Texture();
		virtual ~Texture();
	public:
		static Texture* load(const char* path);
		static Texture* load(Stream* stream);
		static Texture* create(unsigned int width, unsigned int height);
	public:
		bool isRendable() const;
		bool hasBind() const{ return m_textureId != UNDIFINED;} 
		bool keepRawData() const{ return m_keepRawData;}
		void setKeepRawData(bool b) { m_keepRawData = b;}
		void releaseRawData();

		void upScaleTwoPower();
		void flipImageData();


	public:
		unsigned int			m_textureId;
		// 材质的大小可能跟图片不一样
		unsigned int 			m_imageWidth;
		unsigned int			m_imageHeight;
		unsigned int			m_textureWidth;
		unsigned int			m_textureHeight;
		unsigned char *	m_imageData;
		unsigned char		m_alphaType;
		bool 						m_keepRawData;
		unsigned int			m_originalFormat;
		unsigned int			m_channel;

		unsigned int 			m_textureKey;		// 根据路径生成的hash值
		int32						m_textureGLobalIndex;
	};
}

#endif

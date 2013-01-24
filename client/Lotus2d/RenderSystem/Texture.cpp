/********************************************************************
	created:	2012/09/14
	filename: 	Texture.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "Texture.h"
#include "Png/Png.h"
#include "GLHeader.h"
#include "Base/FileStream.h"

namespace Lotus2d {
	Texture::Texture():m_textureId(UNDIFINED), m_keepRawData(false),m_imageData(0)
	{

	}

	Texture::~Texture()
	{
		if(m_textureId!=UNDIFINED){
			glDeleteTextures(1, &m_textureId);
		}
		releaseRawData();
	}

	void Texture::releaseRawData()
	{
		SAFE_FREE(m_imageData);
	}

	bool Texture::isRendable() const
	{
		return (m_textureId!=UNDIFINED) || (m_textureId==UNDIFINED && m_imageData) ;
	}

	Texture* Texture::load(const char* path)
	{
		FileStream stream(path, "rb");
		LOGD("load texture %s", path);
		return load(&stream);
	}

	Texture* Texture::load(Stream* stream)
	{
		GLuint texId = 0;
		Png* png = Png::loadPng(stream);
		if (NULL == png){
			SAFE_DELETE(png);
			return NULL;
		}

		// 鉴于兼容以前的资源暂时不对此做要求
		Texture* texture = new Texture();
		texture->m_keepRawData = 0;
		texture->m_imageWidth = png->m_width;
		texture->m_imageHeight = png->m_height;
		texture->m_textureWidth = png->m_width;
		texture->m_textureHeight = png->m_height;
		texture->m_channel = png->m_channel;

		// 此处channel上还可以做文章

		// TODO:
		texture->m_imageData = (uint8*)lotus2d_malloc((size_t)4 * texture->m_imageWidth * texture->m_imageHeight);
		memcpy(texture->m_imageData,png->m_pixelData,(size_t)4 * texture->m_imageWidth * texture->m_imageHeight);

		texture->m_textureId = UNDIFINED;

		switch(png->m_channel){
		case 1:
			texture->m_originalFormat = GL_LUMINANCE;
			break;
		case 2:
			texture->m_originalFormat = GL_LUMINANCE_ALPHA;
			break;
		case 3:
			texture->m_originalFormat = GL_RGB;
			break;
		case 4:
			texture->m_originalFormat = GL_RGBA;
			break;
		}
		
		// TODO
		texture->m_textureKey = 0; // CSystem::GetIntMD5(filename);

#if LOTUS2D_RENDER !=LOTUS2D_RENDER_OPENGL
		//texture->flipImageData();
		texture->upScaleTwoPower();
#endif

		SAFE_DELETE(png);
		return texture;
	}

	Texture* Texture::create(unsigned int width, unsigned int height)
	{
		//GL_ALPHA,GL_UNSIGNED_BYTE
		
		int new_width = width;
		int new_height = height;

#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_ANDROID
		new_width = 1;
		new_height = 1;

		while( new_width < width )
		{
			new_width *= 2;
		}
		while( new_height < height )
		{
			new_height *= 2;
		}

		//let texture < 2048&2048
		if(new_height>1024||new_width>1024)
		{
			ASSERT(0);//not supprt so large texture
			new_height = height;
			new_width = width;
		}
#endif
		Texture* tex = new Texture();
		glGenTextures(1, &(tex->m_textureId));

		tex->m_keepRawData = 0;
		tex->m_imageWidth = width;
		tex->m_imageHeight = height;
		tex->m_textureWidth = new_width;
		tex->m_textureHeight = new_height;

		glBindTexture(GL_TEXTURE_2D, tex->m_textureId);
#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_ANDROID
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#else	
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#endif		
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			//if(pixeltype==0)
			//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->mTextureWidth, tex->mTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->mImageData);
			//else
				glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, tex->m_textureWidth, tex->m_textureHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, tex->m_imageData);

		return tex;
	}

	void Texture::upScaleTwoPower()
	{
		uint32 new_width = 1;
		uint32 new_height = 1;

		while( new_width < m_imageWidth ){
			new_width *= 2;
		}

		while( new_height < m_imageHeight ){
			new_height *= 2;
		}


		if( (new_width != m_imageWidth) || (new_height != m_imageHeight) ) {
			int heightoffset = new_height - m_imageHeight;
			//int widthoffset = new_width - (*width);
			uint8* image = (unsigned char*)lotus2d_malloc(new_width*new_height*m_channel);
			// 需要重置，否则有灰色印记
			memset(image, 0,new_width*new_height*m_channel);

			for(uint32 i = 0; i < m_imageHeight;i++){
				memcpy(image+new_width*m_channel*i,
					m_imageData+m_imageWidth*m_channel*i, 
					m_imageWidth*m_channel);
			}
			lotus2d_free(m_imageData);
			m_imageData = image;
			m_textureWidth = new_width;
			m_textureHeight = new_height;
		}
	}

	void Texture::flipImageData()
	{
		unsigned int i, j;
		for( j = 0; j*2 < m_imageHeight; ++j )
		{
			int index1 = j * m_imageWidth * m_channel;
			int index2 = (m_imageHeight - 1 - j) * m_imageWidth * m_channel;
			for( i = m_imageWidth * m_channel; i > 0; --i )
			{
				unsigned char temp = m_imageData[index1];
				m_imageData[index1] = m_imageData[index2];
				m_imageData[index2] = temp;
				++index1;
				++index2;
			}
		}
	}
}
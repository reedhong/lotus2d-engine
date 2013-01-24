/********************************************************************
	created:	2012/09/11
	filename: 	ES1Render.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "Render.h"
#include "Base/Macros.h"
#include "GLHeader.h"
#include "string.h"

namespace Lotus2d {
	static GLenum returnGLType(TYPES type)
	{
		switch(type)
		{
		default:
			//	return GL_NONE;

			//case eBOOL:
			//return GL_BOOL;

		case eBYTE:
			return GL_BYTE;

		case eUBYTE:
			return GL_UNSIGNED_BYTE;

		case eSHORT:
			return GL_SHORT;

		case eUSHORT:
			return GL_UNSIGNED_SHORT;


		case eFLOAT:
			return GL_FLOAT;

			//case eDOUBLE:
			//return GL_DOUBLE;
		}
	}

	static GLenum returnPrimitiveType(PRIMITIVE_TYPES type)
	{
		switch(type)
		{
		default:
			//return GL_NONE;

		case ePRIMITIVE_LINES:
			return GL_LINES;

		case ePRIMITIVE_LINE_LOOP:
			return GL_LINE_LOOP;

		case ePRIMITIVE_LINE_STRIP:
			return GL_LINE_STRIP;

		case ePRIMITIVE_TRIANGLES:
			return GL_TRIANGLES;

		case ePRIMITIVE_TRIANGLE_STRIP:
			return GL_TRIANGLE_STRIP;
		}
	}

	static GLenum returnTexFilterMode(TEX_FILTER_MODES mode)
	{
		switch(mode)
		{
		default:
		case eTEX_FILTER_NEAREST:
			return GL_NEAREST;

		case eTEX_FILTER_NEAREST_MIPMAP_NEAREST:
			return GL_NEAREST_MIPMAP_NEAREST;

		case eTEX_FILTER_NEAREST_MIPMAP_LINEAR:
			return GL_NEAREST_MIPMAP_LINEAR;

		case eTEX_FILTER_LINEAR:
			return GL_LINEAR;

		case eTEX_FILTER_LINEAR_MIPMAP_NEAREST:
			return GL_LINEAR_MIPMAP_NEAREST;

		case eTEX_FILTER_LINEAR_MIPMAP_LINEAR:
			return GL_LINEAR_MIPMAP_LINEAR;
		}
	}

	static GLenum returnTexMode(TEX_MODES mode)
	{
		switch(mode)
		{
			//case eR:
			//	return GL_R;

			//case eRG:
			//	return GL_RG;

		default:
		case eRGB:
			return GL_RGB;

		case eRGBA:
			return GL_RGBA;

			//case eDEPTH:
			//	return GL_DEPTH_COMPONENT;
		}
	}

	Render::Render():m_vertexIndex(0), m_primitiveType(ePRIMITIVE_QUARD),m_lineWidth(1)
	{
		for (int i = 0; i < MAX_ARRAY_SIZE * 3 / 2; i+=6 ) {
			int q = i / 6 * 4;
			m_quadIndexes[ i + 0 ] = q + 0;
			m_quadIndexes[ i + 1 ] = q + 1;
			m_quadIndexes[ i + 2 ] = q + 2;
			m_quadIndexes[ i + 3 ] = q + 0;
			m_quadIndexes[ i + 4 ] = q + 2;
			m_quadIndexes[ i + 5 ] = q + 3;
		}
		m_currentTextureId = UNDIFINED;

		m_cameraX = 0;
		m_cameraY = 0;

		m_colorEnable = false;
		m_vertexColor0 = 0xFFFFFFFF;
		m_vertexColor1 = 0xFFFFFFFF;
		m_vertexColor2 = 0xFFFFFFFF;
		m_vertexColor3 = 0xFFFFFFFF;
		disableVertexColor();

		m_clipX = m_clipY = m_clipWidth = m_clipHeight = 0;
		m_needCancelClip = false;
		m_offsetX = m_offsetY = 0;
	}

	void Render::init(int width, int height, float scale)
	{
		m_width = width;
		m_height = height;
		m_clipWidth = m_width;
		m_clipHeight = m_height;
		m_scale =scale;
		glMatrixMode        (GL_MODELVIEW);
		glViewport          (0, 0, width, height);
		glMatrixMode        (GL_MODELVIEW);
		glLoadIdentity      ();

		glEnable( GL_BLEND );
		glEnable			(GL_TEXTURE_2D);
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );


		glColor4f( 1,1,1,1 );

		glMatrixMode        (GL_PROJECTION);
		glLoadIdentity      ();

#if LOTUS2D_RENDER ==LOTUS2D_RENDER_OPENGL
			glOrtho(0, (float)width, (float)height, 0, -99999, 99999);
			//glOrtho			(0, (float)width, 0,(float)height,  -99999, 99999);  // 左下 0，0
#else 	
		glOrthof				(0, (float)width, (float)height, 0, -99999, 99999); // 左上0,0
		//glOrthof	 (0, (float)width, 0,(float)height,  -99999, 99999);  // 左下 0，0
	
#endif


		glDisable				( GL_DEPTH_TEST );
		glDisable				( GL_CULL_FACE );
		glDisable				(GL_LIGHTING);
		glDisableClientState	(GL_NORMAL_ARRAY);
		glDisable				( GL_ALPHA_TEST );

		glMatrixMode        (GL_MODELVIEW);

		resetVertexArrayStatus();

	}


	Render::~Render()
	{

	}

	// instance
	Render * Render::Instance(void)
	{
		static Render s_instance;
		return &s_instance;
	}


	//////////////////////////////////////////////////////////////////////////
	void Render::clearScreen(int argb)
	{
		glClearColor(GL_COLOR_RED_FLOAT(argb), GL_COLOR_GREEN_FLOAT(argb), GL_COLOR_BLUE_FLOAT(argb), GL_COLOR_ALPHA_FLOAT(argb));
		glClear( GL_COLOR_BUFFER_BIT );
	}


	void Render::setColor(int argb)
	{
		glColor4f(GL_COLOR_RED_FLOAT(argb), GL_COLOR_GREEN_FLOAT(argb), GL_COLOR_BLUE_FLOAT(argb), GL_COLOR_ALPHA_FLOAT(argb));
		m_color = argb;
	}

	void Render::setColor(float r, float g, float b, float a)
	{
		glColor4f(r, g, b, a);
		m_color = (((int)(a*255))<<24) + (((int)(r*255))<<16)  + (((int)(g*255))<<8)  + (int)(b*255);
	}

	void Render::setVertexColor(int32 color0,int color1,int color2,int color3)
	{
		m_vertexColor0 = color0;
		m_vertexColor1 = color1;
		m_vertexColor2 = color2;
		m_vertexColor3 = color3;
	}
	void Render::enableVertexColor()
	{
		end();
		m_colorEnable = true;
		setTexEnv(eTEX_ENV_MODULATE);
		glEnableClientState( GL_COLOR_ARRAY );
	}

	void Render::disableVertexColor()
	{	
		end();
		m_colorEnable = false;
		setTexEnv(eTEX_ENV_REPLACE);
		glDisableClientState( GL_COLOR_ARRAY );
	}

	// texture
	void Render::enableTexture(void)
	{
		glEnable(GL_TEXTURE_2D);
	}

	void Render::disableTexture(void)
	{
		glDisable( GL_TEXTURE_2D );
	}
	
	void Render::bindTexture(Texture* texture, BOOL  isLinearFiltering)
	{
		GLuint texId = 0;
		if(!texture->hasBind()){
			glGenTextures( 1, &texId );
			texture->m_textureId = texId;
			glBindTexture(GL_TEXTURE_2D, texId);
			if(isLinearFiltering)
			{		
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);		
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			}
			checkError("bind");
			glTexImage2D(
				GL_TEXTURE_2D, 0,
				texture->m_originalFormat, texture->m_textureWidth, texture->m_textureHeight, 0,
				texture->m_originalFormat, GL_UNSIGNED_BYTE, texture->m_imageData );
			if(!texture->keepRawData()){	// 不保留原始数据
				texture->releaseRawData();
			}
			// 状态切换的时候需要提交显卡
			end();
			m_currentTextureId = texture->m_textureId;
			checkError("bind");
		}else if ( m_currentTextureId != texture->m_textureId){
			m_currentTextureId = texture->m_textureId;
			// 状态切换的时候需要提交显卡
			end();
			glBindTexture(GL_TEXTURE_2D, m_currentTextureId);
			if(isLinearFiltering)
			{		
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);		
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			}
		}
	}

	void Render::texSubImage2D (int xoffset, int yoffset, size_t width, size_t height, const void *pixels)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, width, height, GL_ALPHA, GL_UNSIGNED_BYTE, pixels);
		glPixelStorei(GL_PACK_ALIGNMENT, 4);
	}


	void Render::setTexEnv(TEX_ENV_MODES combine)
	{
		//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);

		switch(combine)
		{
		case eTEX_ENV_REPLACE:
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			break;

		case eTEX_ENV_MODULATE:
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			break;

		case eTEX_ENV_ADD:
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
			break;

		case eTEX_ENV_DECAL:
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			break;
		default:
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			break;
		}
	}


	// arrays
	void Render::enableVertexArray(void) 
	{
		glEnableClientState(GL_VERTEX_ARRAY);
	}

	void Render::enableColorArray(void)
	{
		glEnableClientState(GL_COLOR_ARRAY);
	}

	void Render::enableTexCoordArray(void)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	void Render::disableVertexArray(void)
	{
		glDisableClientState(GL_VERTEX_ARRAY);
	}
	void Render::disableColorArray(void)
	{
		glDisableClientState(GL_COLOR_ARRAY);
	}

	void Render::disableTexCoordArray(void)
	{
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	void Render::setVertexPointer(TYPES type, unsigned int components, const void * pointer)
	{
		glVertexPointer(components, returnGLType(type), 0, pointer);
	}
	void Render::setColorPointer(TYPES type, unsigned int components, const void * pointer)
	{
		glColorPointer(components, returnGLType(type), 0, pointer);
	}

	void Render::setTexCoordPointer(TYPES type, unsigned int components, const void * pointer)
	{
		glTexCoordPointer(components, returnGLType(type), 0, pointer);
	}

	void Render::resetVertexArrayStatus()
	{
		glVertexPointer( 3, GL_FLOAT, sizeof( m_vertexArray[0] ), m_vertexArray );
		glEnableClientState( GL_VERTEX_ARRAY );
		glTexCoordPointer( 2, GL_FLOAT, sizeof( m_texcoordArray[0] ), m_texcoordArray );
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glColorPointer( 4, GL_UNSIGNED_BYTE, sizeof( m_colorArray[0] ), m_colorArray );
	}

	//////////////////////////////////////////////////////////////////////////
	 //draw
	void Render::begin(PRIMITIVE_TYPES type)
	{
		if(m_vertexIndex)
			end();
		m_vertexIndex = 0;
		m_primitiveType = type;
	}

	void Render::vertex3f( float x, float y, float z )
	{
		ASSERT(m_vertexIndex < MAX_ARRAY_SIZE );
		GLfloat v[3] = {x,y,z};
		memcpy(m_vertexArray[m_vertexIndex++],v,sizeof(m_vertexArray[0]));
	}

	void Render::vertex2f( float x, float y )
	{
		ASSERT(m_vertexIndex < MAX_ARRAY_SIZE );
		GLfloat v[3] = {x,y,0};
		memcpy(m_vertexArray[m_vertexIndex++],v,sizeof(m_vertexArray[0]));
	}

	void Render::vertex2i( int x, int y )
	{
		ASSERT(m_vertexIndex < MAX_ARRAY_SIZE );
		GLfloat v[3] = {(float)x,(float)y,0};
		memcpy(m_vertexArray[m_vertexIndex++],v,sizeof(m_vertexArray[0]));
	}

	void Render::texCoord2i( int s, int t )
	{
		ASSERT(m_vertexIndex < MAX_ARRAY_SIZE );
		GLfloat v[2] = {(float)s,(float)t};
		memcpy(m_texcoordArray[m_vertexIndex],v,sizeof(m_texcoordArray[0]));
	}

	void Render::texCoord2f(float s, float t )
	{
		ASSERT(m_vertexIndex < MAX_ARRAY_SIZE );
		GLfloat v[2] = {s, t};
		memcpy(m_texcoordArray[m_vertexIndex],v,sizeof(m_texcoordArray[0]));
	}

	void Render::color4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a )
	{
		ASSERT( m_vertexIndex < MAX_ARRAY_SIZE );
		GLubyte v[4] = {r,g,b,a};
		memcpy(m_colorArray[m_vertexIndex],v,sizeof(m_colorArray[0]));
	}

	void Render::color4ubv( unsigned char *rgba )
	{
		ASSERT( m_vertexIndex < MAX_ARRAY_SIZE );
		memcpy(m_colorArray[m_vertexIndex],rgba,sizeof(m_colorArray[0]));
	}

	void Render::color3f( float r, float g, float b )
	{
		GLubyte v[4] = {(GLubyte) ( r * 255 ),(GLubyte) ( g * 255 ),(GLubyte) ( b * 255 ), 255 };
		color4ubv(v);
	}

	void Render::color4f( float r, float g, float b, float a )
	{
		GLubyte v[4] = {(GLubyte) ( r * 255 ),(GLubyte) ( g * 255 ),(GLubyte) ( b * 255 ),(GLubyte) ( a * 255 )};
		color4ubv(v);
	}

	void Render::end()
	{
		// 如果用到VBO技术就需要打开此处
		//resetVertexArrayStatus();
		if(m_vertexIndex>0)
		{
			if ( m_primitiveType == ePRIMITIVE_QUARD ) {
				glDrawElements( GL_TRIANGLES, m_vertexIndex / 4 * 6, GL_UNSIGNED_SHORT, m_quadIndexes );
			} else {
				glDrawArrays( returnPrimitiveType(m_primitiveType), 0,  m_vertexIndex);
			}
		}
		m_vertexIndex = 0;
		m_primitiveType = ePRIMITIVE_QUARD;//默认GL_QUADS

		checkError("end");

	}

	void Render::drawArray(PRIMITIVE_TYPES type, unsigned int begin, unsigned int size) 
	{
		glDrawArrays(returnPrimitiveType(type), begin, size);
	}
	void Render::drawElement(PRIMITIVE_TYPES type, unsigned int size, TYPES indicesType, const void * indices)
	{
		glDrawElements(returnPrimitiveType(type), size, returnGLType(indicesType), indices);
	}

	void Render::drawRect(int x, int y, int w, int h, int argb)
	{
		// todo
		//if(!isRectInClip(x,y,w,h))
		//	return;

		end();//此行是为了提交以前的材质的东西，防止影响
		glDisable( GL_TEXTURE_2D );
		glDisableClientState (GL_TEXTURE_COORD_ARRAY);
		glEnable(GL_LINE_SMOOTH);
		setColor(argb);

		glLineWidth( 1 );
		begin(ePRIMITIVE_LINES);
		for(int i = 0; i < m_lineWidth;i++)
		{
			vertex2i(i>0?x-1:x, y );
			vertex2i(i>0?x+w+1:x+w, y);

			vertex2i(x+w, y);
			vertex2i( x + w, y + h );

			vertex2i( i>0?x + w+1:x+w, y + h );
			vertex2i( i>0?x-1:x , y+h );

			vertex2i( x , y+h );
			vertex2i(x, y );

			x++;
			y++;
			w-=2;
			h-=2;
		}
		end();
		glLineWidth((float)m_lineWidth );
		glColor4f( 1, 1, 1 ,1);

		glEnable( GL_TEXTURE_2D );
		glEnableClientState (GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_LINE_SMOOTH);
	}

	void Render::drawLine(int x1, int y1, int x2, int y2, int argb, int  lineWidth)
	{
		end();
		glDisable( GL_TEXTURE_2D );
		setColor(argb);
		glLineWidth((float)lineWidth);
		begin(ePRIMITIVE_LINES);
		vertex2i(x1, y1 );
		vertex2i(x2, y2 );
		end();
		glEnable(GL_TEXTURE_2D );
	}
	void Render::drawTexture(const Texture* texture, float x, float y, int transfer, int anchor, float scale)
	{
		if( !texture || !texture->isRendable())
			return ;
		m_tmpImage.m_texture = (Texture *)texture;
		m_tmpImage.setTextureRect(0, 0, (int)(texture->m_textureWidth), 
			(int)(texture->m_textureHeight));
		m_tmpImage.m_width = texture->m_textureWidth*scale;
		m_tmpImage.m_height = texture->m_textureHeight*scale;
		drawImage(&m_tmpImage,x,y,transfer,anchor);
	}

	void Render::drawImage(const Image* image, float x, float y,int transfer,int anchor)
	{
		if(!image|| !image->isRendable())
			return;
		if(anchor!=0){
			if(anchor&ANCHOR_HCENTER){
				x-=image->m_width/2;
			}else if(anchor&ANCHOR_RIGHT){
				x-=image->m_width;
			}

			if(anchor&ANCHOR_VCENTER){
				y-=image->m_height/2;
			}else if(anchor&ANCHOR_BOTTOM){
				y-=image->m_height;
			}
		}


		switch(transfer)
		{
		case TRANS_NONE:
		case TRANS_MIRROR_ROT180:
		case TRANS_MIRROR:
		case TRANS_ROT180:
			{
				if(!isRectInClip(x,y,image->m_width,image->m_height))// for opt
					return;
			}
			break;
		case TRANS_MIRROR_ROT270:
		case TRANS_ROT90:
		case TRANS_ROT270:
		case TRANS_MIRROR_ROT90:
			{
				if(!isRectInClip(x,y,image->m_height,image->m_width))// for opt
					return;
			}
			break;
		}

		bindTexture(image->m_texture);
		// 无顶点颜色
		if(!m_colorEnable){
			switch(transfer)
			{
			case TRANS_NONE:
				texCoord2f( image->m_tx0, image->m_ty0 );	vertex2f(x, y );
				texCoord2f( image->m_tx1, image->m_ty0 );	vertex2f(x + image->m_width, y );
				texCoord2f( image->m_tx1, image->m_ty1 );	vertex2f(x + image->m_width, y + image->m_height);
				texCoord2f( image->m_tx0, image->m_ty1 );	vertex2f(x, y + image->m_height);
				break;
			case TRANS_MIRROR_ROT180:
				texCoord2f( image->m_tx1, image->m_ty0 );	vertex2f(x + image->m_width, y + image->m_height);
				texCoord2f( image->m_tx0, image->m_ty0 );	vertex2f(x, y + image->m_height);
				texCoord2f( image->m_tx0, image->m_ty1 );	vertex2f(x, y );
				texCoord2f( image->m_tx1, image->m_ty1 );	vertex2f(x + image->m_width, y );
				break;
			case TRANS_MIRROR:
				texCoord2f( image->m_tx1, image->m_ty0 );	vertex2f(x, y );
				texCoord2f( image->m_tx0, image->m_ty0 );	vertex2f(x + image->m_width, y );
				texCoord2f( image->m_tx0, image->m_ty1 );	vertex2f(x + image->m_width, y + image->m_height);
				texCoord2f( image->m_tx1, image->m_ty1 );	vertex2f(x, y + image->m_height);

				break;
			case TRANS_ROT180:
				texCoord2f( image->m_tx0, image->m_ty0 );	vertex2f(x + image->m_width, y + image->m_height);
				texCoord2f( image->m_tx1, image->m_ty0 );	vertex2f(x, y + image->m_height);
				texCoord2f( image->m_tx1, image->m_ty1 );	vertex2f(x, y );
				texCoord2f( image->m_tx0, image->m_ty1 );	vertex2f(x + image->m_width, y );
				break;
			case TRANS_MIRROR_ROT270:

				texCoord2f( image->m_tx1, image->m_ty0 );	vertex2f(x, y + image->m_width);
				texCoord2f( image->m_tx0, image->m_ty0 );	vertex2f(x, y );
				texCoord2f( image->m_tx0, image->m_ty1 );	vertex2f(x + image->m_height, y );
				texCoord2f( image->m_tx1, image->m_ty1 );	vertex2f(x + image->m_height, y + image->m_width);

				break;
			case TRANS_ROT90:
				texCoord2f( image->m_tx0, image->m_ty0 );	vertex2f(x + image->m_height, y );
				texCoord2f( image->m_tx1, image->m_ty0 );	vertex2f(x + image->m_height, y + image->m_width); 
				texCoord2f( image->m_tx1, image->m_ty1 );	vertex2f(x, y + image->m_width); 
				texCoord2f( image->m_tx0, image->m_ty1 );	vertex2f(x, y );

				break;
			case TRANS_ROT270:

				texCoord2f( image->m_tx0, image->m_ty0 );	vertex2f(x, y + image->m_width);
				texCoord2f( image->m_tx1, image->m_ty0 );	vertex2f(x, y );
				texCoord2f( image->m_tx1, image->m_ty1 );	vertex2f(x + image->m_height, y );
				texCoord2f( image->m_tx0, image->m_ty1 );	vertex2f(x + image->m_height, y + image->m_width);

				break;
			case TRANS_MIRROR_ROT90:
				texCoord2f( image->m_tx1, image->m_ty0 );	vertex2f(x + image->m_height, y );
				texCoord2f( image->m_tx0, image->m_ty0 );	vertex2f(x + image->m_height, y + image->m_width); 
				texCoord2f( image->m_tx0, image->m_ty1 );	vertex2f(x, y + image->m_width); 
				texCoord2f( image->m_tx1, image->m_ty1 );	vertex2f(x, y );
				break;
			}	
		}else{
			uint8 a0 = (m_vertexColor0>>24)&0xff;
			uint8 r0 = (m_vertexColor0>>16)&0xff;
			uint8 g0 = (m_vertexColor0>>8)&0xff;
			uint8 b0 = m_vertexColor0&0xff;
			uint8 a1 = (m_vertexColor1>>24)&0xff;
			uint8 r1 = (m_vertexColor1>>16)&0xff;
			uint8 g1 = (m_vertexColor1>>8)&0xff;
			uint8 b1 = m_vertexColor1&0xff;
			uint8 a2 = (m_vertexColor2>>24)&0xff;
			uint8 r2 = (m_vertexColor2>>16)&0xff;
			uint8 g2 = (m_vertexColor2>>8)&0xff;
			uint8 b2 = m_vertexColor2&0xff;
			uint8 a3 = (m_vertexColor3>>24)&0xff;
			uint8 r3 = (m_vertexColor3>>16)&0xff;
			uint8 g3 = (m_vertexColor3>>8)&0xff;
			uint8 b3 = m_vertexColor3&0xff;

			switch(transfer)
			{
			case TRANS_NONE:
				texCoord2f( image->m_tx0, image->m_ty0 );	color4ub(r0,g0,b0,a0); vertex2f(x, y );
				texCoord2f( image->m_tx1, image->m_ty0 );	color4ub(r1,g1,b1,a1);vertex2f(x + image->m_width, y );
				texCoord2f( image->m_tx1, image->m_ty1 );	color4ub(r2,g2,b2,a2);vertex2f(x + image->m_width, y + image->m_height);
				texCoord2f( image->m_tx0, image->m_ty1 );	color4ub(r3,g3,b3,a3);vertex2f(x, y + image->m_height);
				break;
			case TRANS_MIRROR_ROT180:
				texCoord2f( image->m_tx1, image->m_ty0 );	color4ub(r1,g1,b1,a1);vertex2f(x + image->m_width, y + image->m_height);
				texCoord2f( image->m_tx0, image->m_ty0 );	color4ub(r0,g0,b0,a0);vertex2f(x, y + image->m_height);
				texCoord2f( image->m_tx0, image->m_ty1 );	color4ub(r3,g3,b3,a3);vertex2f(x, y );
				texCoord2f( image->m_tx1, image->m_ty1 );	color4ub(r2,g2,b2,a2);vertex2f(x + image->m_width, y );
				break;
			case TRANS_MIRROR:

				texCoord2f( image->m_tx1, image->m_ty0 );	color4ub(r1,g1,b1,a1);vertex2f(x, y );
				texCoord2f( image->m_tx0, image->m_ty0 );	color4ub(r0,g0,b0,a0);vertex2f(x + image->m_width, y );
				texCoord2f( image->m_tx0, image->m_ty1 );	color4ub(r3,g3,b3,a3);vertex2f(x + image->m_width, y + image->m_height);
				texCoord2f( image->m_tx1, image->m_ty1 );	color4ub(r2,g2,b2,a2);vertex2f(x, y + image->m_height);

				break;
			case TRANS_ROT180:
				texCoord2f( image->m_tx0, image->m_ty0 );	color4ub(r0,g0,b0,a0);vertex2f(x + image->m_width, y + image->m_height);
				texCoord2f( image->m_tx1, image->m_ty0 );	color4ub(r1,g1,b1,a1);vertex2f(x, y + image->m_height);
				texCoord2f( image->m_tx1, image->m_ty1 );	color4ub(r2,g2,b2,a2);vertex2f(x, y );
				texCoord2f( image->m_tx0, image->m_ty1 );	color4ub(r3,g3,b3,a3);vertex2f(x + image->m_width, y );
				break;
			case TRANS_MIRROR_ROT270:

				texCoord2f( image->m_tx1, image->m_ty0 );	color4ub(r1,g1,b1,a1);vertex2f(x, y + image->m_width);
				texCoord2f( image->m_tx0, image->m_ty0 );	color4ub(r0,g0,b0,a0);vertex2f(x, y );
				texCoord2f( image->m_tx0, image->m_ty1 );	color4ub(r3,g3,b3,a3);vertex2f(x + image->m_height, y );
				texCoord2f( image->m_tx1, image->m_ty1 );	color4ub(r2,g2,b2,a2);vertex2f(x + image->m_height, y + image->m_width);

				break;
			case TRANS_ROT90:
				texCoord2f( image->m_tx0, image->m_ty0 );	color4ub(r0,g0,b0,a0);vertex2f(x + image->m_height, y );
				texCoord2f( image->m_tx1, image->m_ty0 );	color4ub(r1,g1,b1,a1);vertex2f(x + image->m_height, y + image->m_width); 
				texCoord2f( image->m_tx1, image->m_ty1 );	color4ub(r2,g2,b2,a2);vertex2f(x, y + image->m_width); 
				texCoord2f( image->m_tx0, image->m_ty1 );	color4ub(r3,g3,b3,a3);vertex2f(x, y);

				break;
			case TRANS_ROT270:
				texCoord2f( image->m_tx0, image->m_ty0 );	color4ub(r0,g0,b0,a0);vertex2f(x, y + image->m_width);
				texCoord2f( image->m_tx1, image->m_ty0 );	color4ub(r1,g1,b1,a1);vertex2f(x, y);
				texCoord2f( image->m_tx1, image->m_ty1 );	color4ub(r2,g2,b2,a2);vertex2f(x + image->m_height, y );
				texCoord2f( image->m_tx0, image->m_ty1 );	color4ub(r3,g3,b3,a3);vertex2f(x + image->m_height, y + image->m_width);

				break;
			case TRANS_MIRROR_ROT90:
				texCoord2f( image->m_tx1, image->m_ty0 );	color4ub(r1,g1,b1,a1);vertex2f(x + image->m_height, y);
				texCoord2f( image->m_tx0, image->m_ty0 );	color4ub(r0,g0,b0,a0);vertex2f(x + image->m_height, y + image->m_width); 
				texCoord2f( image->m_tx0, image->m_ty1 );	color4ub(r3,g3,b3,a3);vertex2f(x, y + image->m_width); 
				texCoord2f( image->m_tx1, image->m_ty1 );	color4ub(r2,g2,b2,a2);vertex2f(x, y );
				break;
			}
		}
	}

	void Render::drawImageWithColor(const Image* image, float x, float y,int color1,int color2)
	{

		uint8 a0 = (color1>>24)&0xff;
		uint8 r0 = (color1>>16)&0xff;
		uint8 g0 = (color1>>8)&0xff;
		uint8 b0 = color1&0xff;
		uint8 a1 = (color2>>24)&0xff;
		uint8 r1 = (color2>>16)&0xff;
		uint8 g1 = (color2>>8)&0xff;
		uint8 b1 = color2&0xff;

		if(!isRectInClip(x,y,image->m_width,image->m_height))
			return;

		bindTexture(image->m_texture);

		texCoord2f( image->m_tx0, image->m_ty0 );	color4ub(r0,g0,b0,a0);	vertex2f(x, y );
		texCoord2f( image->m_tx1, image->m_ty0 );	color4ub(r0,g0,b0,a0);	vertex2f(x + image->m_width, y );
		texCoord2f( image->m_tx1, image->m_ty1 );	color4ub(r1,g1,b1,a1);	vertex2f(x + image->m_width, y + image->m_height);
		texCoord2f( image->m_tx0, image->m_ty1 );	color4ub(r1,g1,b1,a1);	vertex2f(x, y + image->m_height);
	}



	
	// masks
	void Render::setColorMask(bool r, bool g, bool b, bool a)
	{
		glColorMask(r, g, b, a);
	}

	// alpha
	void Render::setAlphaTest(float value)
	{
		if(value > 0.0f)
			glEnable(GL_ALPHA_TEST);
		else {
			glDisable(GL_ALPHA_TEST);
		}

		glAlphaFunc(GL_GREATER, value);
	}


	// matrix
	void Render::loadIdentity(void) 
	{
		glLoadIdentity();
	}
	void Render::setMatrixMode(MATRIX_MODES mode) 
	{
		switch(mode)
		{
		case eMATRIX_MODELVIEW:
			glMatrixMode(GL_MODELVIEW);
			return;

		case eMATRIX_PROJECTION:
			glMatrixMode(GL_PROJECTION);
			return;

		case eMATRIX_TEXTURE:
			glMatrixMode(GL_TEXTURE);
			return;
		}
	}
	void Render::pushMatrix(void) 
	{
		glPushMatrix();
	}
	void Render::popMatrix(void) 
	{
		glPopMatrix();
	}

	// blending
	void Render::enableBlending(void) 
	{

	}
	void Render::disableBlending(void) 
	{

	}
	void Render::setBlendingMode(BLENDING_MODES mode) 
	{

	}

	bool Render::isRectInClip(float x, float y, float w, float h)
	{
		if(!((x<m_clipX&&x+w<m_clipX)||(x>m_clipX+m_clipWidth)
			||(y<m_clipY&&y+h<m_clipY)||(y>m_clipY+m_clipHeight)))
			return true;
		else
		{
			Matrix44f modelViewMatrix;
			glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix.arr);
			if(modelViewMatrix.arr[0]!=1.0||modelViewMatrix.arr[5]!=1.0)
				return TRUE;
			if(modelViewMatrix.rstrans.translation.xyzw.x||modelViewMatrix.rstrans.translation.xyzw.y)
				return TRUE;
		}
		return FALSE;
	}

	void Render::setClip(int x, int y, int w, int h)
	{
		if(w<=0||h<=0)
			return;

		if(x != m_clipX || y != m_clipY || w != m_clipWidth || h != m_clipHeight){

			Matrix44f modelViewMatrix;
			end();
			glEnable( GL_SCISSOR_TEST );
			glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix.arr);

			if(modelViewMatrix.arr[0]!=1.0||modelViewMatrix.arr[5]!=1.0){
				x = (int)(x*modelViewMatrix.arr[0]);
				y = (int)(y*modelViewMatrix.arr[5]);
				w = (int)(w*modelViewMatrix.arr[0]);
				h = (int)(h*modelViewMatrix.arr[5]);
			}


			x = (int)(x+modelViewMatrix.rstrans.translation.xyzw.x);
			y = (int)(y+modelViewMatrix.rstrans.translation.xyzw.y);


			if(x<0) { w+=x; x=0; }
			if(y<0) { h+=y; y=0; }
			if(x+w > m_width) w=m_width-x;
			if(y+h > m_height) h=m_height-y;

			if(w<=0)
				w = 1;
			if(h<=0)
				h = 1;

			glScissor((GLint) x, (GLint)(m_height- (y + h)),  (GLsizei)w, (GLsizei)h );

			m_needCancelClip = true;
			m_clipX = x; 
			m_clipY = y;
			m_clipWidth = w;
			m_clipHeight = h;
		}
	}

	void Render::disableClip()
	{
		if(m_needCancelClip){
			end();
			glDisable( GL_SCISSOR_TEST);
			m_needCancelClip = false;
		}
		m_clipX = 0; 
		m_clipY = 0;
		m_clipWidth = m_width;
		m_clipHeight = m_height;
	}

	void Render::fillRect( int x,int y, int w, int h, int argb)
	{
		end();//此行是为了提交以前的材质的东西，防止影响
		if(w>0&&h>0){
			glEnable( GL_SCISSOR_TEST );
			glScissor( x+m_offsetX, m_height-(y+h+m_offsetY), w, h );

			glClearColor( GL_COLOR_RED_FLOAT(argb),  GL_COLOR_GREEN_FLOAT(argb),
				GL_COLOR_BLUE_FLOAT(argb), GL_COLOR_ALPHA_FLOAT(argb));
			glClear( GL_COLOR_BUFFER_BIT );
			glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

			glDisable( GL_SCISSOR_TEST );
		}
	}

	void Render::getClipArea(int *clipX, int *clipY, int *clipWidth, int *clipHeight)
	{
		*clipX = m_clipX; *clipY = m_clipY;
		*clipWidth = m_clipWidth; *clipHeight = m_clipHeight;
	}


	void Render::setOffset(int x, int y)
	{
		m_offsetX = x;
		m_offsetY = y;
	}
	

	void Render::checkError(const char* message)
	{
		GLint err = glGetError();
		if ( err != GL_NO_ERROR ) {
			LOGD( "GL ERROR %d from %s\n", err, message );
		}
	}
}
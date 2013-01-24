/********************************************************************
	created:	2012/09/11
	filename: 	ES1Render.h
	author:		reedhong
	
	purpose:	
*********************************************************************/

#ifndef __Lotus2d_ES1Render_H__
#define __Lotus2d_ES1Render_H__

#include "Base/Config.h"
#include "Image.h"
#include "Texture.h"

namespace Lotus2d {

	#define GET_SCALE_FACTOR()  (Render::Instance()->getScale())

	enum BLENDING_MODES
	{
		eBLENDING_NONE = 0,
		eBLENDING_ALPHA,
		eBLENDING_ADD,
		eBLENDING_SUB,
		eBLENDING_LIGHT,
		eBLENDING_PRODUCT
	};

	// depth modes
	enum DEPTH_MODES
	{
		eDEPTH_NONE = 0,
		eDEPTH_ALWAYS,
		eDEPTH_LESS,
		eDEPTH_GREATER,
		eDEPTH_EQUAL,
		eDEPTH_LEQUAL,
		eDEPTH_GEQUAL,
		eDEPTH_NOTEQUAL
	};

	// matrix modes
	enum MATRIX_MODES
	{
		eMATRIX_MODELVIEW = 0,
		eMATRIX_PROJECTION,
		eMATRIX_TEXTURE,
	};

	// primitives types
	enum PRIMITIVE_TYPES
	{
		ePRIMITIVE_POINTS = 0,
		ePRIMITIVE_LINES,
		ePRIMITIVE_LINE_LOOP,
		ePRIMITIVE_LINE_STRIP,
		ePRIMITIVE_TRIANGLES,
		ePRIMITIVE_TRIANGLE_STRIP,
		ePRIMITIVE_TRIANGLE_FAN,
		ePRIMITIVE_QUARD
	};



	// texture image mode
	enum TEX_MODES
	{
		eDEPTH = 0,
		eR = 1,
		eRG = 2,
		eRGB = 3,
		eRGBA = 4
	};

	// texture gen modes
	enum TEX_GEN_MODES
	{
		eTEX_GEN_NONE = 0,
		eTEX_GEN_SPHERE_MAP,
		eTEX_GEN_CUBE_MAP
	};

	// texture combine modes
	enum TEX_ENV_MODES
	{
		eTEX_ENV_REPLACE = 0,
		eTEX_ENV_MODULATE,
		eTEX_ENV_ADD,
		eTEX_ENV_DECAL
	};

	// texture filtering modes
	enum TEX_FILTER_MODES
	{
		eTEX_FILTER_NEAREST = 0,
		eTEX_FILTER_NEAREST_MIPMAP_NEAREST,
		eTEX_FILTER_NEAREST_MIPMAP_LINEAR,
		eTEX_FILTER_LINEAR,
		eTEX_FILTER_LINEAR_MIPMAP_NEAREST,
		eTEX_FILTER_LINEAR_MIPMAP_LINEAR,
	};

	// wrap modes
	enum WRAP_MODES
	{
		eWRAP_REPEAT = 0,
		eWRAP_CLAMP
	};

	// cull modes
	enum CULL_MODES
	{
		eCULL_NONE = 0,
		eCULL_FRONT,
		eCULL_BACK,
		eCULL_FRONT_BACK
	};

	// types
	enum TYPES
	{
		eBOOL = 0,
		eBYTE,
		eUBYTE,
		eSHORT,
		eUSHORT,
		eINT,
		eUINT,
		eFLOAT
	};
	class Render
	{
	private:
		Render();
	public:
		 ~Render();
		 // instance
		 static Render * Instance(void);
	protected:
		int	m_color;// 按照argb存储，在一个整数里面
		float m_texcoordArray[MAX_ARRAY_SIZE][2];
		char m_colorArray[MAX_ARRAY_SIZE][4];
		float m_vertexArray[MAX_ARRAY_SIZE][3];
		short m_quadIndexes[MAX_ARRAY_SIZE * 3 / 2 ];	// 索引数组
		PRIMITIVE_TYPES m_primitiveType;
		int m_vertexIndex;		//  当前是第几个顶点
		int m_lineWidth;
		int m_width;
		int m_height;
		float m_scale;
		int m_currentTextureId; // 记录当前的纹理id

		// 启用顶点颜色
		BOOL m_colorEnable;
		int m_vertexColor0, m_vertexColor1, m_vertexColor2, m_vertexColor3;

		// drawTexture时，需要根据参数重新调整image的属性，用这个作为零时转化用途
		Image m_tmpImage;

		// clip
		int m_clipX, m_clipY, m_clipWidth, m_clipHeight;
		bool m_needCancelClip;

		int m_offsetX, m_offsetY;
	// 2d游戏中的camera
	public:
		int m_cameraX;
		int m_cameraY;

		void setCamera(int x, int y) 
		{
			m_cameraX = x;
			m_cameraY = y;
		}
		void moveCamera(int offsetX, int offsetY)
		{
			m_cameraX += offsetX;
			m_cameraY += offsetY;
		}
		int getCameraX() { return m_cameraX;}
		int getCameraY() { return m_cameraY;}

	public:
		// init
		void init(int width, int height, float scale=1.0);

		int getScreenWidth() { return m_width;}
		int getScreenHeight() { return m_height;}
		float getScale() { return m_scale;}

		int getLineWidth() { return m_lineWidth;}
		void setLineWidth(int width) { m_lineWidth = width;}
		// clear
		void clearScreen(int argb);

		// color 
		int getColor() { return m_color;}
		void setColor(int color);
		void setColor(float r, float g, float b, float a);


		void setVertexColor(int32 color0,int color1,int color2,int color3);
		void enableVertexColor();
		void disableVertexColor();


		// texture
		void enableTexture(void);
		void disableTexture(void);
		/*
		 * 绑定材质，如果没有提交显卡，需要生成材质id，指定纹理数据，进行提交
		 */
		void  bindTexture(Texture* texture, BOOL  isLinearFiltering=TRUE);
		
		/*
		 * 修改材质数据
		 */
		void texSubImage2D (int xoffset, int yoffset, size_t width, size_t height, const void *pixels);
		void setTexEnv(TEX_ENV_MODES combine);


		//  vertex arrays
		void enableVertexArray(void) ;
		void enableColorArray(void);
		void enableTexCoordArray(void);

		void disableVertexArray(void);
		void disableColorArray(void);
		void disableTexCoordArray(void);

		void setVertexPointer(TYPES type, unsigned int components, const void * pointer);
		void setColorPointer(TYPES type, unsigned int components, const void * pointer);
		void setTexCoordPointer(TYPES type, unsigned int components, const void * pointer);
		void resetVertexArrayStatus();

		// draw
		void begin(PRIMITIVE_TYPES type);
		void vertex3f( float x, float y, float z );
		void vertex2f( float x, float y );
		void vertex2i( int x, int y );
		void texCoord2i( int s, int t );
		void texCoord2f(float s, float t );
		void color4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a );
		void color4ubv( unsigned char *rgba );
		void color3f( float r, float g, float b );
		void color4f( float r, float g, float b, float a );
		void end();

		void drawArray(PRIMITIVE_TYPES type, unsigned int begin, unsigned int size) ;
		void drawElement(PRIMITIVE_TYPES type, unsigned int size, TYPES indicesType, const void * indices) ;
		void drawRect(int x, int y, int w, int h, int argb);
		void drawLine(int x1, int y1, int x2, int y2, int argb, int lineWidth=1);
		void drawImage(const Image* image, float x, float y,int transfer=TRANS_NONE,int anchor=ANCHOR_TOP|ANCHOR_LEFT);
		void drawImageWithColor(const Image* image, float x, float y,int color1,int color2);
		void drawTexture(const Texture* texture, float x, float y,int transfer=TRANS_NONE,int anchor=ANCHOR_TOP|ANCHOR_LEFT, float scale=1.0);
		



		// masks
		void setColorMask(bool r, bool g, bool b, bool a);
		// alpha
		void setAlphaTest(float value);

		// matrix
		void loadIdentity(void) ;
		void setMatrixMode(MATRIX_MODES mode) ;
		void pushMatrix(void) ;
		void popMatrix(void) ;

		#if 0
		void multMatrix(const MMatrix4x4 * matrix) = 0;
		void translate(const MVector3 & position) = 0;
		void rotate(const MVector3 & axis, float angle) = 0;
		void scale(const MVector3 & scale) = 0;
		void getViewport(int * viewport) = 0;
		void getModelViewMatrix(MMatrix4x4 * matrix) = 0;
		void getProjectionMatrix(MMatrix4x4 * matrix) = 0;
		void getTextureMatrix(MMatrix4x4 * matrix) = 0;
		#endif

		// blending
		void enableBlending(void) ;
		void disableBlending(void) ;
		void setBlendingMode(BLENDING_MODES mode) ;

		// clip
		bool isRectInClip(float x, float y, float w, float h);
		void setClip(int x, int y, int w, int h);
		void disableClip();
		void fillRect( int x,int y, int w, int h, int argb);
		void getClipArea(int *clipX, int *clipY, int *clipWidth, int *clipHeight);

		// UI绘制的时候需要移动坐标，所以，加上这个参数
		void setOffset(int x, int y);

	
		// checkError
		void checkError(const char* message);
	};
}

#endif 

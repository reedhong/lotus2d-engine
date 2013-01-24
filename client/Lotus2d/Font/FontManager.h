/********************************************************************
	created:	2012/09/16
	filename: 	FontManager.h
	author:		reedhong
	
	purpose:	字体管理： 
						todo: 考虑内存池的机制，避免重复请求和分配，很有必要
*********************************************************************/

#ifndef __Lotus2d_FontManager_H__
#define __Lotus2d_FontManager_H__

#include "Base/Prerequisites.h"
#include "Base/ObjectBase.h"
#include "RenderSystem/Texture.h"
#include "RenderSystem/Image.h"
#include "Base/UString.h"
#include "Base/MMapStream.h"

#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_STROKER_H
#include FT_GLYPH_H
#include FT_TRUETYPE_IDS_H


namespace Lotus2d {	
	class FontManager 
	{
	public:
		class FontGlyphContainer;

		class FontGlyph:public ObjectBase
		{
		public:
			FontGlyph(int16 ch,FontGlyphContainer* pcontainer);
			~FontGlyph();
			void setImageInfo(int x, int y, int width, int height);
		public:
			/**
			* The image for this glyph.
			*/
			Image *m_glyphImage;

			/**
			* The image for this glyph border.
			*/
			Image *m_glyphImageBorder;
			/**
			* The unicode character for this glyph.
			*/
			unsigned short m_character;
			/**
			* The cached advance for this glyph.
			*/
			uint8 m_advance;
			
			FontGlyphContainer* m_container;
		};

		class FontGlyphContainer:public ObjectBase
		{
		public:
			FontGlyphContainer(int font_size, FONT_CACHE_IMAGE_SIZE cache_tex_mode,int border, bool bold);
			~FontGlyphContainer();

		
			size_t getHeight();
			size_t getStringWidth(const UString& str);

			/*
			 * 最核心的接口: 获得字体的字形图
			 */
			FontGlyph* getGlyph(unsigned short ch);	
			/*
			 * 最核心的接口: 渲染字符串
			 */
			void render(const UString& str,int x, int y, bool isHorizontal, int anchor_mode,  int trans_mode, float scale);
		public:
			/**
			* The texture this glyph container owned.
			*/
			Texture* m_cachedTexture;
			/**
			 * The texture width for the cached texture.
			 */
			int m_textureWidth;
			/**
			 * The texture height for the cached texture.
			 */
			int m_textureHeight;
			/**
			 * The current cursor for texture caching reservation.
			 */
			unsigned int m_currentCursor;

			/**
			 * Is the ascii character be directly mapped to the 
			 * cached texture.
			 */
			bool is_ASCII_direct_mapping;

			/**
			 * The max char width with current font size.
			 */
			int m_maxCharWidth;
			/**
			 * The max char height with current font size.
			 */
			int m_maxCharHeight;
			/**
			 * The column counts with the char width & height
			 * of current font size for the cached texture.
			 */
			int m_columnCounts;
			/**
			* The row counts with the char width & height
			* of current font size for the cached texture.
			*/
			int m_rowCounts;
			/**
			 * The max chars the cached texture can hold for.
			 */
			unsigned int m_maxCharCounts;

			/**
			 * The font size this glyph container holds glphys for.
			 */
			int m_fontSize;
			/**
			 * The font height for the font size.
			 */
			int m_fontHeight;

			/**
			 * The container to hold glyphs.
			 * map(Key: char code => Value: ge_glyph*)
			 */
			map<unsigned short, FontGlyph*> m_glyphMap;
			/**
			 * The character array.
			 */
			vector<unsigned short> m_characters;

			/**
			 *  border width
			 */
			uint8 m_borderWidth;
			
			/**
			 *  has bold
			 */
			bool m_bold;

			int m_verticalGap;
		};

	protected:
		FontManager();
	public:
		~FontManager();
		static FontManager * Instance(void);
		bool load(const char* file);
		void release();
		bool createFont(int  size,FONT_CACHE_IMAGE_SIZE chachesize,uint8 border,bool bold);
		/*
		 * @str: 需要绘制的字符串，为utf8格式，通过此获得对于的unicode码，通过unicode码获得字符图元
		 * @x, y, 位置起始坐标
		 * @isHorizontal： 是否为水平方向，也可以是垂直方向
		 * @anchor_mode: 对齐模式
		 * @trans_mode: 翻转
		 * @scale: 缩放
		 */
		void draw(const UString & str,  int x, int y, bool isHorizontal=true, int anchor_mode=ANCHOR_NONE, 
				int trans_mode=TRANS_NONE, float scale=1.0f);
		int getStringWidth(const UString& str);
		int getCharWidth(int16 ch);
		// 获得当前字体的高度
		int getFontHeight();
		bool setFont(int  fontSize);
		void setBorderColor(int color);
		void setColor(int color);
		void setGradual(bool enable,int32 topcolor,int bottomcolor);

	protected:
		MMapStream m_stream;
		/**
		* The current font size.
		*/
		int m_currentFontSize;
		/**
		* The current font color.
		*/ 
		int m_currentFontColor;
		// 渐变
		bool m_hasGradual;

		/**
		* The current font top color. 上渐变色
		*/ 
		int m_topColor;

		/**
		* The current font bottom color.下渐变色
		*/ 
		int m_bottomColor;

		/**
		* The current font border color.
		*/ 
		int m_borderColor;

		/**
		* The current font container.
		*/
		FontGlyphContainer* m_currentFontContainer;

		/**
		* batch draw font.
		*/
		bool m_batchdraw;
		/**
		* Should the font be rendered to the bitmap with antialiase?
		*/
		bool m_isAntialias;
		/**
		* Is the tt font loaded?
		*/
		bool m_isLoaded;

		/**
		* Holds the glyph container of different face size.
		*/
		vector<FontGlyphContainer*> m_containers;

		/**
		* The free type library.
		*/
		FT_Library m_ftLibrary;
		/**
		* The free type face.
		*/
		FT_Face m_ftFace;			
	};
}

#endif

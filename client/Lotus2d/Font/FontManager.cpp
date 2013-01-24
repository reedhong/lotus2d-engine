/********************************************************************
	created:	2012/09/16
	filename: 	FontManager.cpp
	author:		reedhong
	
	purpose:	字体管理
*********************************************************************/

#include "FontManager.h"
#include "RenderSystem/Render.h"
#include "Base/FileStream.h"

namespace Lotus2d {

	//////////////////////////////////////////////////////////////////////////
	// FontManager::ge_font_glyph
	FontManager::FontGlyph::FontGlyph(int16 ch, FontGlyphContainer* pcontainer):
		m_glyphImage(0),m_glyphImageBorder(0),m_character(ch), m_advance(0),m_container(pcontainer)
	{
	}

	FontManager::FontGlyph::~FontGlyph()
	{
		SAFE_DELETE(m_glyphImage) ;
		SAFE_DELETE(m_glyphImageBorder) ;
	}

	void FontManager::FontGlyph::setImageInfo(int x, int y, int width, int height)
	{
		ASSERT(m_container);
		SAFE_DELETE(m_glyphImage);
		SAFE_DELETE(m_glyphImageBorder);

		m_glyphImage =  new Image(m_container->m_cachedTexture, x,y,width,height);
		if(!m_container->m_borderWidth){
			m_glyphImageBorder = NULL;
		}else{
			y += m_container->m_rowCounts*m_container->m_maxCharHeight;
			m_glyphImageBorder = new Image(m_container->m_cachedTexture, x,y,width,height);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// FontManager::ge_font_glyph_container
	FontManager::FontGlyphContainer::FontGlyphContainer(int size, FONT_CACHE_IMAGE_SIZE cache_tex_mode,int border, bool bold):m_fontSize(size),m_borderWidth(border), m_bold(bold)
	{
		switch(cache_tex_mode)
		{
		case FONT_CACHE_IMAGE_64x64:		///< 64x64
			m_textureWidth = m_textureHeight = 64;
			break;
		case FONT_CACHE_IMAGE_128x128:		///< 128x128
			m_textureWidth = m_textureHeight = 128;
			break;
		case FONT_CACHE_IMAGE_256x256:		///< 256x256
			m_textureWidth = m_textureHeight = 256;
			break;
		case FONT_CACHE_IMAGE_512x512: 		///< 512x512
			m_textureWidth = m_textureHeight = 512;
			break;
		case FONT_CACHE_IMAGE_1024x1024:	///< 1024x1024
		default:
			m_textureWidth = m_textureHeight = 1024;
			break;
		}

		if(m_borderWidth)
			m_textureWidth = m_textureHeight = m_textureWidth*2;
		if(m_textureWidth>1024)
			m_textureWidth = m_textureHeight = 1024;
		
		m_cachedTexture = Texture::create(m_textureWidth, m_textureHeight);//,GL_ALPHA,GL_UNSIGNED_BYTE);

		/* Calculate dimension */
		m_currentCursor = 0;
		m_maxCharHeight = m_maxCharWidth = m_fontSize + TEX_FONT_GLYPH_PADDING+2*border; 
		m_columnCounts = (m_textureWidth / m_maxCharWidth);
		m_rowCounts = (m_textureHeight / m_maxCharHeight);
		if(m_borderWidth){
			if(m_rowCounts%2)
				m_rowCounts--;
			m_rowCounts = m_rowCounts/2;
		}

		m_maxCharCounts = m_columnCounts * m_rowCounts;
		m_fontSize = m_fontSize;
		m_verticalGap = 4;
		m_fontHeight = m_maxCharHeight;
	}

	FontManager::FontGlyphContainer::~FontGlyphContainer()
	{
		map<unsigned short, FontGlyph*>::iterator iter  = m_glyphMap.begin();
		for(; iter != m_glyphMap.end(); ++iter){
			SAFE_DELETE(iter->second);
		}
		SAFE_RELEASE(m_cachedTexture);
	}

	// todo: 实现得比较隐晦，待修正， font_height跟max_char_height有何差异
	size_t FontManager::FontGlyphContainer::getHeight()
	{
		return m_fontHeight;
	}

	size_t FontManager::FontGlyphContainer::getStringWidth(const UString& str)
	{
		if( str.empty()) return 0;

		size_t strLenght = str.length();
		int width = 0;
		/* Draw each string char. */
		for(size_t i = 0; i < strLenght; i++){
			int index = 0;
			unsigned short ch = str.get(i);
			FontGlyph* ch_glyph = getGlyph(ch);

			if(ch_glyph){
				width += ch_glyph->m_advance;
			}
		}
		return width;
	}

	FontManager::FontGlyph* FontManager::FontGlyphContainer::getGlyph(unsigned short ch)
	{
		if(ch == 0){  /* Invalid character code */
			return NULL;
		}

		map<unsigned short, FontManager::FontGlyph*>::iterator iter  = m_glyphMap.find(ch);
		if( iter != m_glyphMap.end()){
			return m_glyphMap[ch];
		}
		/* No longer Ascii direct mapping anymore. */
		// 相当于废弃掉了
		if(is_ASCII_direct_mapping){
			is_ASCII_direct_mapping = FALSE;
			m_currentCursor = 0;
		}

		/* Load the character glyph info through freetype lib. */
		FontManager* fontManager = FontManager::Instance();
		FT_GlyphSlot slot = fontManager->m_ftFace->glyph;

		int render_flag = FT_LOAD_RENDER;
		int x = 0, y = 0;
		slot->format = FT_GLYPH_FORMAT_OUTLINE;

		/* In case out of bound */
		if(m_currentCursor >= m_maxCharCounts){
			m_currentCursor = 0;
		}

		/* Calculate the position of this charecter code in the texture. */
		y = (m_currentCursor / m_columnCounts) * m_maxCharHeight; 
		x = (m_currentCursor % m_columnCounts) * m_maxCharWidth;
		//x = (gc->current_cursor % gc->row_counts) * gc->max_char_width;

		/* Antialiase */
		if(!fontManager->m_isAntialias){
			render_flag |= FT_LOAD_TARGET_MONO;
		}

		/* Load character code */
		if(FT_Load_Char(fontManager->m_ftFace, ch, render_flag))
			return NULL;


		int top = m_fontSize - slot->bitmap_top + 1;
		int offset = top * m_maxCharWidth + slot->bitmap_left + 2;
		int rows = m_maxCharHeight - top - 1;

		int offset_x;
		int offset_y;

		FT_Stroker stroker;
		FT_Error error;
		FT_Glyph bitmap_glyph = NULL;
		FT_Glyph bitmap_glyph_border = NULL;
		FT_Bitmap* bitmap =  &slot->bitmap; 
		FT_Bitmap* bitmapborder =  NULL; 
		slot->format = FT_GLYPH_FORMAT_OUTLINE;

		//FT_Bitmap_Embolden(gc->owner_font->ft_library,&slot->bitmap,12,12);
		if(m_bold||m_borderWidth>0){
			if(m_bold){
				FT_Outline_Embolden(&slot->face->glyph->outline,70);
			}
			FT_Get_Glyph( slot, &bitmap_glyph );
			if(m_borderWidth>0){
				FT_Get_Glyph( slot, &bitmap_glyph_border );

				error = FT_Stroker_New(fontManager->m_ftLibrary, &stroker );
				FT_Stroker_Set( stroker, m_borderWidth<<6, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0 ); 
				FT_Glyph_StrokeBorder( &bitmap_glyph_border, stroker,0 ,1 );
				FT_Stroker_Set( stroker, 0, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0 ); 
				FT_Glyph_StrokeBorder( &bitmap_glyph, stroker,0 ,1 );
				FT_Stroker_Done( stroker );

				FT_Glyph_To_Bitmap( &bitmap_glyph_border,  ft_render_mode_normal, 0, 1 );
				bitmapborder = &((FT_BitmapGlyph)bitmap_glyph_border)->bitmap;
			}

			FT_Glyph_To_Bitmap( &bitmap_glyph,  ft_render_mode_normal, 0, 1 );

			if(bitmap_glyph_border)
			{
				offset_x = ((FT_BitmapGlyph)bitmap_glyph)->left - ((FT_BitmapGlyph)bitmap_glyph_border)->left;
				offset_y = ((FT_BitmapGlyph)bitmap_glyph)->top - ((FT_BitmapGlyph)bitmap_glyph_border)->top;
				if(offset_x!=0||offset_y!=0)
				{
					int i = 0;
					i++;
				}
			}
			bitmap = &((FT_BitmapGlyph)bitmap_glyph)->bitmap;
		}

		top = slot->bitmap.rows - slot->bitmap_top + 1;
		offset = top * m_maxCharWidth + slot->bitmap_left + 2;
		rows = m_maxCharHeight - top - 1;

		uint8* texBuffer =(uint8*)lotus2d_malloc(sizeof(uint8) * m_maxCharHeight * m_maxCharWidth);
		if(texBuffer == NULL){
			return NULL;
		}
		// todo: 这个地方后续可以优化，memset是一个非常消耗资源的操作，看是否可以干掉
		memset(texBuffer, 0x0, sizeof(uint8) * m_maxCharHeight * m_maxCharWidth);

		/* Gray mode  defualt*/
		if(m_borderWidth>0&&bitmapborder&&bitmapborder){
			top = m_fontSize - ((FT_BitmapGlyph)bitmap_glyph_border)->top;

			if(top<0){
				top = 0;
			}
			offset = top * m_maxCharWidth + ((FT_BitmapGlyph)bitmap_glyph_border)->left + 2;
			if(offset<0){
				offset = 0;
			}
			rows = m_maxCharHeight - top - 1;
			for(int i = 0; i < bitmapborder->rows && rows >= 0; i++){
				for(int j=0; j < bitmapborder->width && j < m_maxCharWidth; j++){
					uint8 grey = bitmapborder->buffer[i * bitmapborder->width + j];
					if(grey&&i * m_maxCharWidth + j + offset>=0&&i * m_maxCharWidth + j + offset<m_maxCharWidth*m_maxCharHeight){
						texBuffer[i * m_maxCharWidth + j + offset] = grey;
					}
				}
				rows--;
			}
			//如果还没有bind到显卡的要先bind到显卡
			Render::Instance()->bindTexture(m_cachedTexture);
			Render::Instance()->texSubImage2D(x, y+m_rowCounts*m_maxCharHeight, m_maxCharWidth, m_maxCharHeight,  texBuffer);
			memset(texBuffer, 0x0, sizeof(uint8) * m_maxCharHeight * m_maxCharWidth);
		}
		if(bitmap_glyph){
			top = m_fontSize - ((FT_BitmapGlyph)bitmap_glyph)->top;
		}else{
			top = m_fontSize - slot->bitmap_top;
		}
		if(top<0){
			top = 0;
		}
		if(bitmap_glyph){
			offset = top * m_maxCharWidth + ((FT_BitmapGlyph)bitmap_glyph)->left + 2;
		}else{
			offset = top * m_maxCharWidth + slot->bitmap_left + 2;
		}
		ASSERT(offset>=0);
		if(offset<0){
			offset = 0;
		}
		rows = m_maxCharHeight - top - 1;

		for(int i = 0; i < bitmap->rows && rows >= 0; i++){
			for(int j = 0; j < bitmap->width && j < m_maxCharWidth; j++){
				uint8 grey = bitmap->buffer[i * bitmap->width + j];
				if(grey&&i *m_maxCharWidth + j + offset>=0&&i * m_maxCharWidth + j + offset<m_maxCharWidth*m_maxCharHeight)				{
					texBuffer[i *m_maxCharWidth + j + offset] = grey;
				}
			}
			rows--;
		}

		if(bitmap_glyph)
			FT_Done_Glyph(bitmap_glyph);
		if(bitmap_glyph_border)
			FT_Done_Glyph(bitmap_glyph_border);

		if(m_currentCursor < m_characters.size()){
			/* Set the character code info. */
			unsigned short oldch  = m_characters[m_currentCursor];
			m_characters[m_currentCursor] = ch;

			map<unsigned short, FontGlyph*>::iterator iter  = m_glyphMap.find(oldch);
			if( iter != m_glyphMap.end()){
				FontGlyph* glyph = iter->second;
				m_glyphMap.erase(iter);
				SAFE_DELETE(glyph) 
			}
		}
		else{
			m_characters.push_back(ch);
		}

		FontGlyph* ch_glyph = new FontGlyph(ch, this);
		m_glyphMap.insert(make_pair(ch, ch_glyph));
		ch_glyph->m_advance = (uint8)(slot->advance.x >> 6);
		if(m_borderWidth)
			ch_glyph->m_advance += m_borderWidth;
		ch_glyph->setImageInfo(x , y+1, ch_glyph->m_advance + 4, m_maxCharHeight-1);


		/* Texture this font bp */
		//glBindTexture(GL_TEXTURE_2D, gc->cached_texture->mTextureId);
		
		Render::Instance()->bindTexture(m_cachedTexture);
		Render::Instance()->texSubImage2D(x, y, m_maxCharWidth, m_maxCharHeight, texBuffer);
		lotus2d_free(texBuffer);
		m_currentCursor++;
		return ch_glyph;
	}


	void FontManager::FontGlyphContainer::render(const UString & str,   int x, int y, bool isHorizontal, int anchor_mode,  int trans_mode, float scale)
	{
		if( str.empty()) return ;

		size_t str_len = str.length();
		float posX = (float)x;
		float posY = (float)y;

		int i = 0;
		int rotate_degree = 0;
		bool mirror = false;
 
#if 0  // 暂时废弃掉这些变换，很容易出错,基本上没太大作用 added by reedhong 2012.11.21
		if(anchor_mode != ANCHOR_NONE || trans_mode != TRANS_NONE){
			int width = getStringWidth(str);
			int height = getHeight();
			Util::geoTransform(&x, &y, &width, &height, &rotate_degree, &mirror, trans_mode, anchor_mode);
		}
#endif
		// TODO handle transforming.

		FontManager* fontManger = FontManager::Instance();

		if(!fontManger->m_batchdraw){
			Render::Instance()->begin( ePRIMITIVE_QUARD );
			Render::Instance()->setTexEnv(eTEX_ENV_MODULATE);
			if(fontManger->m_hasGradual){
				Render::Instance()->enableVertexColor();
			}
		}
		/* Draw each string char. */
		int currentColor = Render::Instance()->getColor();
		// 描边
		if(m_borderWidth){
			
			Render::Instance()->setColor(fontManger->m_borderColor);

			// todo: 此处有优化的空间，在获取utf-8字符串的第i个unicode字符是，通过循环不断计算而来
			// 想想挺浪费计算的,优化之，只需要一次循环
			for(size_t i = 0; i < str_len; i++){
				unsigned short ch = str.get(i);
				FontGlyph* ch_glyph = getGlyph(ch);
				if(ch_glyph){
					float advance = ch_glyph->m_advance * scale;
					if(!fontManger->m_hasGradual){
						Render::Instance()->drawImage(ch_glyph->m_glyphImageBorder, posX, posY, 0, 0);
					}else{
						// 渐变
						Render::Instance()->drawImageWithColor(ch_glyph->m_glyphImageBorder, posX, posY, 
							fontManger->m_borderColor,fontManger->m_borderColor);
					}
					if(isHorizontal){
						posX += advance;
					}else{
						posY += advance + m_verticalGap;
					}
				}
			} // End for.
			if(!fontManger->m_batchdraw){
				Render::Instance()->end();
			}
			
		}
		// 重置
		posX = (float)x;

		// 绘制字体

		Render::Instance()->setColor(fontManger->m_currentFontColor);
		for(size_t i = 0; i < str_len; i++){
			unsigned short ch = str.get(i);
			FontGlyph* ch_glyph = getGlyph(ch);
			if(ch_glyph){
				float advance = ch_glyph->m_advance * scale;
				if(!fontManger->m_hasGradual){
					Render::Instance()->drawImage(ch_glyph->m_glyphImage, posX, posY, 0, 0);
				}else{
					Render::Instance()->drawImageWithColor(ch_glyph->m_glyphImage, posX, posY,
						fontManger->m_topColor,fontManger->m_bottomColor);
				}
				if(isHorizontal){
					posX += advance;
				}else{
					posY += advance + m_verticalGap;
				}
			}
		} 
		
		//todo:
		if(!fontManger->m_batchdraw){
			Render::Instance()->end();
			Render::Instance()->setTexEnv(eTEX_ENV_REPLACE);
			if(fontManger->m_hasGradual){
				Render::Instance()->disableVertexColor();
			}
		}
		Render::Instance()->setColor(currentColor);
	}


	//////////////////////////////////////////////////////////////////////////
	// FontManager
	FontManager::FontManager():m_isLoaded(FALSE)
	{

	}

	FontManager::~FontManager()
	{
		release();
	}
	FontManager * FontManager::Instance(void)
	{
		static FontManager s_instance;
		return &s_instance;
	}

	bool FontManager::load(const char* file)
	{
		ASSERT(!m_isLoaded);
		if(!file) return FALSE;
		if(FT_Init_FreeType(&m_ftLibrary) != 0) return FALSE;

		bool bRet = m_stream.open(file);
		ASSERT(bRet==TRUE);

		int ret = FT_New_Memory_Face(m_ftLibrary, m_stream.getBuffer() , m_stream.size(), 0, &m_ftFace);
		ASSERT(ret == 0);

		m_isAntialias = TRUE;
		m_isLoaded = TRUE;

		return TRUE;
	}

	void FontManager::release()
	{
		if(m_ftFace != NULL){
			FT_Done_Face(m_ftFace);
		}

		if(m_ftLibrary != NULL){
			FT_Done_FreeType(m_ftLibrary);
		}

		vector<FontGlyphContainer* >::iterator iter = m_containers.begin();
		for( ; iter != m_containers.end(); ++iter){
			delete (*iter);
		}
		m_containers.clear();
		m_stream.close();
	}

	bool FontManager::createFont(int size,FONT_CACHE_IMAGE_SIZE chachesize,uint8 border,bool bold)
	{	
		size_t count = m_containers.size();
		for(size_t i = 0; i < count; i++){
			FontGlyphContainer *glyph_container =m_containers[i];
			if(glyph_container->m_fontSize == size){
				ASSERT(0); // todo:
				break;
			}
		}
		FontGlyphContainer* container = new FontGlyphContainer(size, chachesize, border, bold);
		m_containers.push_back(container);
		return TRUE;
	}

	void FontManager::draw(const UString & str,  int x, int y, bool isHorizontal, int anchor_mode,
		int trans_mode, float scale)
	{
		m_currentFontContainer->render(str, x, y, isHorizontal, anchor_mode, TRANS_NONE, scale);
	}	

	int FontManager::getStringWidth(const UString& str)
	{
		return m_currentFontContainer->getStringWidth(str);
	}

	int FontManager::getFontHeight()
	{
		return m_currentFontContainer->m_fontHeight;
	}

	int FontManager::getCharWidth(int16 ch)
	{
		return 0;
	}

	bool FontManager::setFont(int fontSize)
	{
		vector<FontGlyphContainer* >::iterator iter = m_containers.begin();
		for( ; iter != m_containers.end(); ++iter){
			FontGlyphContainer* container = *iter;
			if(container->m_fontSize == fontSize){
				if (FT_Set_Pixel_Sizes(m_ftFace, container->m_fontSize, container->m_fontSize) == 0){
					FT_Set_Transform(m_ftFace, 0, 0);
					m_currentFontSize = container->m_fontSize;
					m_currentFontContainer = container;
					return TRUE;
				}
			}
		}

		return FALSE;
	}

	void FontManager::setBorderColor(int color)
	{
		m_borderColor = color;
	}

	void FontManager::setGradual(bool enable,int32 topcolor,int bottomcolor)
	{
		m_hasGradual = enable;
		m_topColor = topcolor;
		m_bottomColor = bottomcolor;
	}

	void FontManager::setColor(int color)
	{
		m_currentFontColor = color;
	}
}
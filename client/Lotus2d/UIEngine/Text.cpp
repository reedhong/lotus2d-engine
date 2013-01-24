/********************************************************************
	created:	2012/11/08
	filename: 	Text.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "Text.h"
#include "ControlDef.h"

namespace Lotus2d {

	Text::Text()
	{
		m_type = eCtrlText;
		m_fontAttribute.align = ANCHOR_CENTER;
		m_fontAttribute.size = MIDDLE_FONT;
	}

	Text::~Text()
	{

	}

	Text* Text::copy()
	{
		Text* clone = new Text();
		return copy(clone);
	}

	Text* Text::copy(Text* clone)
	{
		Control::copy(clone);
		clone->m_content = m_content;
		memcpy((void *)&(clone->m_fontAttribute), (void *)&m_fontAttribute, sizeof(FontAttribute));

		return clone;
	}

	void Text::loadImp(Stream* stream) 
	{	
		m_content = stream->readUString();
		m_fontAttribute.color = stream->read32le();
		m_fontAttribute.borderColor = stream->read32le();
		m_fontAttribute.size = stream->read8();
		m_fontAttribute.align = stream->read8();
		m_fontAttribute.isHorizontal = stream->read8() == 1;
	}

	void Text::drawImp(int offsetX, int offsetY)
	{
		if(m_content.empty())
			return ;

		// todo: 支持多行

		FontManager::Instance()->setFont(m_fontAttribute.size);
		FontManager::Instance()->setColor(m_fontAttribute.color);
		FontManager::Instance()->setBorderColor(m_fontAttribute.borderColor);
		
		//int anchor;
		int x, y;

		int stringHeight =  FontManager::Instance()->getFontHeight(); 
		int stringWidth = FontManager::Instance()->getStringWidth(m_content);

		if (m_fontAttribute.align == eAlignLeft){
			x = m_x + offsetX; 
		}else if (m_fontAttribute.align == eAlignRight){
			x = m_x + offsetX + m_width - stringWidth; 
		}else {
			x = m_x + offsetX + ((m_width - stringWidth)>>1);
		}
		y =  m_y +offsetY + ((m_height-stringHeight)>>1);
		FontManager::Instance()->draw(m_content, x, y, m_fontAttribute.isHorizontal, ANCHOR_NONE);
	}

	bool Text::handleEventImp(eActionEventType event, int posX, int posY, int offsetX,int offsetY)
	{
		return false;
	}

	void Text::setText(const UString& content)
	{
		m_content = content;
	}
}
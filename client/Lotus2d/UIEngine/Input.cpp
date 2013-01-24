/********************************************************************
	created:	2012/11/08
	filename: 	Input.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "Input.h"
#include "Platform/textfield.h"
#include "RenderSystem/Render.h"

namespace Lotus2d {

	static Input* s_editor;
	static void _onInputMethodChange()
	{
		if(s_editor)
			s_editor->onInputMethodChange();
	}

	static void _onTextChange(char* utf8)
	{
		if(s_editor)
			s_editor->onTextChange(utf8);
	}

	static void _onInputFinished()
	{
		if(s_editor)
			s_editor->onTextFinished();
	}

	static void _onInputkey(int key)
	{
		if(s_editor)
			s_editor->onInputKey(key);
	}

	static void _onInpututf8(char* utf8)
	{
		if(s_editor)
			s_editor->onInputUtf8(utf8);
	}


	Input::Input()
	{
		m_type = eCtrlInput;
		m_focusable = true;
		m_gap = INPUT_CONTENT_GAP;
		m_maxlen =INPUT_DEFAULT_MAX_LEN;
		m_firstClick = true;
		m_isFocused = false;
		m_drawTick = 0;
		m_doRemoveFocus = false;
		m_inputmode = eInputNormal;
		m_disable = false;
	
		//继承自 IInputEditObserver
		inputMethodChange = _onInputMethodChange;
		textChange = _onTextChange;
		inputkey = _onInputkey;
		inpututf8 = _onInpututf8;
		inputFinished = _onInputFinished;
	}
	Input::~Input()
	{

	}

	Input* Input::copy()
	{
		Input* clone = new Input();

		Text::copy(clone);

		clone->m_drawTick = m_drawTick;
		clone->m_firstClick = m_firstClick;
		clone->m_doRemoveFocus = m_doRemoveFocus;
		clone->m_disable = m_disable;		// 不可输入
		clone->m_maxlen = m_maxlen;		// 字符个数，而不是buf
		clone->m_inputmode = m_inputmode;
		clone->m_gap = m_gap;	// 头尾隔开的距离

		return clone;
	}

	void Input::loadImp(Stream* stream) 
	{
		Text::loadImp(stream);

		m_maxlen = stream->read16le();
		m_gap = stream->read16le();
		m_inputmode = stream->read8();
		m_disable = stream->read8()==1;
	}


	void Input::drawImp(int offsetX, int offsetY)
	{
		if(m_doRemoveFocus){
			setFocus(false);
			m_doRemoveFocus = false;
		}

		Render::Instance()->fillRect(m_x+offsetX,  m_y+offsetY, m_width, m_height, 0xFFCCCCCC);
		Render::Instance()->drawRect(m_x+offsetX,  m_y+offsetY, m_width, m_height, 0xFFCCCCCC);
		

		int lineX = 0;
		int fontHeight = FontManager::Instance()->getFontHeight();
		if(!m_content.empty()){
			Render::Instance()->setClip(m_x + offsetX, m_y + offsetY,m_width,m_height);
			UString displayStr = m_content;
			if ( m_inputmode == eInputPassword){
				displayStr = UString('*', m_content.length());
			}

			int strWidth = FontManager::Instance()->getStringWidth(displayStr);
			int displayWidth  = m_width -m_gap*2- INPUT_TICK_GAP;
			int startX = m_x+offsetX;
			if(strWidth >  displayWidth){
				lineX = m_x + offsetX + displayWidth + INPUT_TICK_GAP + m_gap;
				startX -= strWidth-displayWidth;
			}else{
				lineX = m_x + offsetX + strWidth + INPUT_TICK_GAP+m_gap;
			}
			FontManager::Instance()->setFont(m_fontAttribute.size);
			FontManager::Instance()->setColor(m_fontAttribute.color);
			FontManager::Instance()->setBorderColor(m_fontAttribute.borderColor);
			FontManager::Instance()->draw(displayStr, startX+m_gap, 
				m_y+offsetY+ ((m_height-fontHeight)/2), true, ANCHOR_NONE);
			Render::Instance()->disableClip();
		}

#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_WIN32
		if(m_isFocused){
			if(m_drawTick<15){
				int offset = m_height-fontHeight;
				Render::Instance()->drawLine(lineX, m_y+offsetY+offset/4, lineX, 
					m_y+offsetY+m_height-offset/4, INPUT_TICK_COLOR,2);
			}
		}
		m_drawTick++;
		if(m_drawTick>30)
				m_drawTick = 0;
#endif
	}

	bool Input::handleEventImp(eActionEventType event, int posX, int posY, int offsetX,int offsetY)
	{
		if(m_disable)
			return false;

		if (event == eActionDown){
			LOGD("in input::handleEventImp %d, %p", m_isFocused, this);
			if (!m_isFocused){
				setFocus(true,offsetX,offsetY);		
			}
			return true;
		}
		return false;
	}

	void Input::setFocus(bool focus,int offsetX,int offsetY)
	{
		if(!m_focusable)
			return ;

		LOGD("in input::setFocus %d, %p", focus, this);
		if (focus){
			if (m_firstClick){
				setText(UString(""));
				m_firstClick = false;
			}
			if (m_isFocused){
				return;
			}
			if(m_inputmode == eInputPassword){
				setText(UString(""));
			}
			if (s_editor && s_editor!=this){
				s_editor->setFocus(false);
			}
			s_editor = this;
			g_inputEditor.observer = this;   // for win32
			// 加载钩子
			if(g_inputEditor.delegate) // ios & android
			{
#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_IOS
				g_inputEditor.delegate->_setVisiable(TRUE);
#endif		
#if LOTUS2D_PLATFORM ==  LOTUS2D_PLATFORM_ANDROID     
				float scale = Render::Instance()->getScale();
				g_inputEditor.delegate->_setExtent((m_x+offsetX)*scale, (m_y+offsetY)*scale, (m_width)*scale, m_height*scale);
				if(!m_content.empty()){
					g_inputEditor.delegate->_setText(m_content.c_str());
				}
#endif             

#if LOTUS2D_PLATFORM ==  LOTUS2D_PLATFORM_IOS             
				g_inputEditor.delegate->_setExtent(m_x + offsetX,m_y + offsetY, m_width, m_height);
				if(!m_content.empty()){
					g_inputEditor.delegate->_setText(m_content.c_str());
				}
#endif       
                if (m_inputmode == eInputNormal) {
                    g_inputEditor.delegate->_setInputMethod(E_TF_IM_ALL);
                }
                else  if (m_inputmode == eInputNumber) {
                    g_inputEditor.delegate->_setInputMethod(E_TF_IM_NUM);
                }
                else {
                    g_inputEditor.delegate->_setInputMethod(E_TF_IM_ABC);
                }
				g_inputEditor.delegate->_setInputPassword(m_inputmode==eInputPassword);
				g_inputEditor.delegate->_setInputMaxLength(m_maxlen);
				// todo:
				g_inputEditor.delegate->_setInputColor(INPUT_COLOR);

#if LOTUS2D_PLATFORM ==  LOTUS2D_PLATFORM_ANDROID     
				g_inputEditor.delegate->_setVisiable(TRUE);		
#endif		               
			}
		}else{
			if (!m_isFocused){
				return;
			}
			// 卸载钩子
			if(g_inputEditor.delegate){	
				/*
				 * IOS中，响应TextChange事件就可以获得这个值，但在andriod，没法直接激活
				 * 所以需要通过代理控件获取
				 */
				char* str = g_inputEditor.delegate->_getText();
				// 不得已写这个，因为内部的钩子是用malloc分配的 added by reedhong 2012.12.1
				UString ustr(str);
				free(str);
				setText(ustr);
				// todo:  如何删除str
				// 
				g_inputEditor.delegate->_setInputPassword(FALSE);
				g_inputEditor.delegate->_setInputMethod(E_TF_IM_ALL);
				g_inputEditor.delegate->_setExtent(0,0,1,1);
				g_inputEditor.delegate->_setVisiable(FALSE);
			}		
		}

		m_isFocused = focus;
	}

	void Input::onInputMethodChange()
	{

	}

	void Input::onTextChange(char* utf8)
	{
		if (utf8) {
			LOGD("Input::onTextChange %s", utf8);
			m_content = utf8;
		}else{
			LOGD("Input::onTextChange %s", "null");
			m_content = "";
		}
	}

	void Input::onTextFinished()
	{
		LOGD("Input::onTextFinished %s", "");
		m_doRemoveFocus = true;
	}

	void Input::onInputKey(int key)
	{
		if(key>=32&&key<128){  // 字符串
			if( m_content.length() >= m_maxlen)
				return ;
			char ch = key;
			if (m_inputmode == eInputNumber){
				if(key >=48 && key <= 57 ){
					char ch = key;
					m_content.append(ch);
				}
			}else{
				m_content.append(ch);
			}
		}else{	// 命令
			if(key==8 && !m_content.empty()) {//win delete key			
				int contentLen = m_content.length();
				m_content = m_content.substr(0, contentLen-1);		
			}
		}
	}

	void Input::onInputUtf8(char* utf8)
	{
		m_content.append(utf8);
	}
}
/********************************************************************
	created:	2012/11/08
	filename: 	Input.h
	author:		reedhong
	
	purpose:	输入框
*********************************************************************/

#ifndef __Lotus2d_Input_H__
#define __Lotus2d_Input_H__

#include "Text.h"
#include "ControlDef.h"
#include "Font/FontManager.h"
#include "Platform/textfield.h"

namespace Lotus2d {

	class Input: public Text,  public IInputEditObserver
	{
	public:
		Input();
		virtual ~Input();
		Input* copy();

		/* 
		 * 从stream中加载控件信息
		 */
		virtual void loadImp(Stream* stream) ;
		/*
		 * 绘制控件
		 */
		virtual void drawImp(int posX = 0, int posY = 0);
		virtual bool handleEventImp(eActionEventType event, int posX, int posY, int offsetX=0,int offsetY=0);
		void setFocus(bool focus,int offsetX=0,int offsetY=0);
		void setDisable(bool b) { m_disable = b;}
		virtual bool hasChild() { return false;}

		void onInputMethodChange();
		void onTextChange(char* utf8);
		void onTextFinished();
		void onInputKey(int key);
		void onInputUtf8(char* utf8);

	private:  // 内部状态
		int m_drawTick;

		bool m_firstClick;
		bool m_doRemoveFocus;
	public:
		bool	m_disable;		// 不可输入
		unsigned int		m_maxlen;		// 字符个数，而不是buf
		unsigned char	m_inputmode;
		int		m_gap;	// 头尾隔开的距离

	};

}

#endif
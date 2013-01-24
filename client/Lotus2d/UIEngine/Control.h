/********************************************************************
	created:	2012/09/22
	filename: 	Control.h
	author:		reedhong
	
	purpose:	UI控件的基类
*********************************************************************/

#ifndef __Lotus2d_Control_H__
#define __Lotus2d_Control_H__

#include "Base/ObjectBase.h"
#include "Base/Prerequisites.h"
#include "Base/Stream.h"
#include "RenderSystem/Sprite.h"
#include "Event/Event.h"
#include "TinyXML/tinyxml.h"
#include "ControlDef.h"

namespace Lotus2d  {
	class Control: public ObjectBase
	{
	public:
		Control();
		virtual ~Control();
		virtual Control* copy() = 0;
		Control* copy(Control* clone);



		virtual void loadImp(Stream* stream)  = 0;
		virtual void drawImp(int posX = 0, int posY = 0)=0;
		virtual bool handleEventImp(eActionEventType event, int posX, int posY, int offsetX=0,int offsetY=0)=0;

		// 给UI引擎用的接口
		virtual void  loadFromXML(TiXmlElement *elem){}
		virtual TiXmlElement*  saveToXML(){ return 0;}
		virtual bool hasChild()=0;
		virtual Control* getFirstChild(){ return NULL;}
		virtual Control* getNextChild() { return NULL;}
		virtual Control* getParent() { return m_parent;}
		virtual void addChild(Control* control) {}
		virtual void deleteChild(Control* control) {}
		virtual void forwardControl(Control* control, bool isup){}
		void getControlByPos(Control** control, int x, int y, int offsetX=0, int offsetY=0);
	

		void load(Stream* stream) ;
		void draw(int posX, int posY);
		bool handleActionEvent(eActionEventType event, int posX, int posY, int offsetX=0,int offsetY=0);
		void setPos(int x, int y);
		void setWH(int w, int h);
		void setRect(int x, int y, int w, int h);
		void setVisible(bool b);
		void setFocusAble(bool b);
		void setFocused(bool b);
		void setShowBorder(bool b);

		void setBGSprite(Sprite* sprite);

		string getName( ) { return m_name;}
		void setName(const string& name);
	private:
		void dispatchEvent(eCtrlEventType type);
	public:
		unsigned char		m_type;
		int		m_x;
		int		m_y;
		int		m_width;
		int		m_height;
		short	m_mainEvent;			// 主要的事件号

		int		m_bgSpriteKey;		// 背景精灵
		int		m_aframeIndex;		// 一般的静态帧
		int		m_focusedAframeIndex;	// 选中状态的静态帧

		bool m_visible;				// 是否可见
		bool m_focusable;		// 是否可选中
		bool m_isFocused;			// 是否被选中

		bool m_showBorder;	// 显示四边形边框

		Control* m_parent;
	public:
		string	m_mainEventName; //  主事件名，用于UI编辑器做标识
	private:
		string	m_name;

	private:
		Sprite*	m_bgSprite;

	};
}

#endif
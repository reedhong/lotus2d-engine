/********************************************************************
	created:	2012/09/22
	filename: 	Control.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/
#include "Base/Util.h"
#include "Control.h"
#include "ControlDef.h"
#include "RenderSystem/Render.h"
#include "UIEngine.h"
#include "Resource/ResManagerBase.h"

namespace Lotus2d {
	
	Control::Control()
	{
		m_type = eCtrlBase;
		m_x = 0;
		m_y = 0;
		m_width = 0;
		m_height = 0;
		m_mainEvent = 0;
		m_bgSpriteKey = 0;		// 背景精灵
		m_aframeIndex = -1;		// 一般的静态帧
		m_focusedAframeIndex = -1;	// 选中状态的静态帧


		m_visible = true;
		m_focusable = false;
		m_isFocused = false;

		m_parent = NULL;
		m_bgSprite = NULL;
	}

	Control::~Control()
	{
		SAFE_RELEASE(m_bgSprite);
	}


	Control* Control::copy(Control* clone)
	{
		clone->m_type = m_type;
		clone->m_x = m_x;
		clone->m_y = m_y;
		clone->m_width = m_width;
		clone->m_height = m_height;
		clone->m_mainEvent = m_mainEvent;
		clone->m_visible = m_visible;
		clone->m_focusable = m_focusable;
		clone->m_isFocused = m_isFocused;
		clone->m_parent = m_parent;
		return clone;
	}

	void Control::load(Stream* stream)
	{
		assert(stream != NULL);
		m_x = (int)(stream->read16le() * GET_SCALE_FACTOR());
		m_y =  (int)(stream->read16le() * GET_SCALE_FACTOR());
		m_width =  (int)(stream->read16le() * GET_SCALE_FACTOR());
		m_height =  (int)(stream->read16le() * GET_SCALE_FACTOR());
		m_mainEvent = (int)stream->read16le();

		m_bgSpriteKey =  (int)stream->read32le();
		m_aframeIndex = (int)stream->read16le();
		m_focusedAframeIndex = (int)stream->read16le();
		
		if(m_bgSpriteKey > 0){
			setBGSprite((Sprite *)ResourceManagerBase::getInstance()->loadResouce(m_bgSpriteKey, eResSprite));
		}

		m_visible = (stream->read8()==1);
		m_focusable = (stream->read8()==1);


		loadImp(stream);

		// todo: 边框开关
		m_showBorder = false;
	}



	void Control::draw(int posX, int posY)
	{
		if(!m_visible)
			return ;
		if(m_showBorder){
			Render::Instance()->drawRect(posX+m_x, posY+m_y, m_width, m_height, 0xFF000000);
		}
		if(m_bgSprite){
			int size =  m_bgSprite->getSFrameCount();
			if(m_isFocused && 0<= m_focusedAframeIndex && m_focusedAframeIndex < size){
					m_bgSprite->drawSFrame(m_focusedAframeIndex, posX+m_x, posY+m_y);
			}else{
				if(0<= m_aframeIndex && m_aframeIndex < size){
					m_bgSprite->drawSFrame(m_aframeIndex, posX+m_x, posY+m_y);
				}
			}
		}
		drawImp(posX, posY);
	}	

	bool Control::handleActionEvent(eActionEventType event, int posX, int posY, int offsetX,int offsetY)
	{
		if( !m_visible || !m_focusable){
			return false;
		}
		if(!Util::isInRect(posX,posY,m_x + offsetX,m_y + offsetY,m_width,m_height)){
			return false;
		}
		// 点击事件
		if(m_mainEvent > 0 && event == eActionUp){
			UIEngine::Instance()->handleUICommand(m_mainEvent, this);
		}

		return handleEventImp(event, posX, posY, offsetX, offsetY);
	}

	void Control::getControlByPos(Control** selectedControl,int x, int y, int offsetX, int offsetY)
	{
		if( hasChild()){
			vector<Control *> childs;
			Control* child = getFirstChild();
			while(child){
				//child->getControlByPos(control, x, y, m_x+offsetX, m_y+offsetY);
				childs.push_back(child);
				child = getNextChild();
			}
			// 逆序访问
			for(unsigned int i = childs.size(); i >0; i--){
				child = childs[i-1];
				child->getControlByPos(selectedControl, x, y, m_x+offsetX, m_y+offsetY);
			}
		}
		if(*selectedControl == NULL &&Util::isInRect(x, y, m_x+offsetX, m_y+offsetY, m_width, m_height)){
			*selectedControl  = this;
			return ;
		}
	}

	void Control::setPos(int x, int y)
	{
		m_x = x;
		m_y = y;
	}

	void Control::setWH(int w, int h)
	{
		m_width = w;
		m_height = h;
	}

	void Control::setRect(int x, int y, int w, int h)
	{
		m_x = x;
		m_y = y;
		m_width = w;
		m_height = h;
	}

	void Control::setVisible(bool b)
	{
		m_visible = b;
	}

	void Control::setFocusAble(bool b)
	{
		m_focusable = b;
	}

	void Control::setShowBorder(bool b)
	{
		m_showBorder = b;
	}

	void Control::setFocused(bool b)
	{
		m_isFocused  = b;
	}

	void Control::setBGSprite(Sprite* sprite)
	{
		SAFE_RELEASE(m_bgSprite);
		m_bgSprite = sprite;
		SAFE_ADDREF(m_bgSprite);
	}

	void Control::setName(const string& name)
	{
		m_name = name;
	}

}
/********************************************************************
	created:	2012/10/22
	filename: 	UILayer.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "UILayer.h"
#include "Screen.h"
#include "Constants/GameConfig.h"

#include"Base/FileStream.h"
using namespace Lotus2d;

#include "ResManager/ResManager.h"
#include "Network/NetworkManager.h"
#include "Game.h"

UILayer::UILayer()
{
	
}

UILayer::~UILayer()
{

}

void UILayer::enter()
{
}
 
void UILayer::leave()
{
	SAFE_DELETE(m_currentPage);
}

void UILayer::step()
{

}

// todo:
int UILayer::handleEvent(eActionEventType event, int x, int y)
{
	bool bRet = m_currentPage->handleActionEvent(event, x, y);
	if(bRet){
		return 0;
	}
	return 1;
}

void UILayer::drawImp()
{
	if(m_currentPage){
		m_currentPage->draw(0, 0);
	}
}

void UILayer::setCurrentPage(unsigned int key)
{
	// todo: ÊÍ·Å
	m_currentPage = ResManager::Instance()->getPage(key);
}

/********************************************************************
	created:	2012/10/23
	filename: 	TestLayer.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/


#include "TestLayer.h"
#include "Font/FontManager.h"
#include "Generate/R.h"

#include "ResManager/ResManager.h"
#include "Game.h"
#include "Camera.h"



TestLayer::TestLayer()
{
	
}

TestLayer::~TestLayer()
{

}

void TestLayer::enter()
{
}
 
void TestLayer::leave()
{

}

void TestLayer::step()
{
	m_actor->step();
}


int TestLayer::handleEvent(eActionEventType event, int x, int y)
{
	return -1;
}

void TestLayer::drawImp()
{
	m_patchSprite->drawPatch(1,0,0, 640, 480);
	m_actor->paint();

	const UString& confirm = ResManager::Instance()->getString(0);
	FontManager::Instance()->setColor(R::color::FONT);
	FontManager::Instance()->setBorderColor(R::color::FONT_BORDER);
	FontManager::Instance()->draw(confirm, 0, 50,400,ANCHOR_NONE);
}
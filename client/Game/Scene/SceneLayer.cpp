/********************************************************************
	created:	2012/10/22
	filename: 	SceneLayer.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "SceneLayer.h"
#include "Constants/GameConfig.h"
#include "ResManager/ResManager.h"
#include "Camera.h"

#include "TinyXML/tinyxml.h"
#include "Base/Memory.h"
using namespace  std;

#include "Audio/AudioEngine.h"
#include "RenderSystem/Render.h"
using namespace std;

SceneLayer::SceneLayer():m_collides(NULL),m_terrain(0),m_hero(0)
{
	
}

SceneLayer::~SceneLayer()
{

}

void SceneLayer::enter()
{
}
 
void SceneLayer::leave()
{

}

void SceneLayer::step()
{
	//actors
	vector<Actor * >::iterator iter = m_actors.begin();
	for(; iter != m_actors.end(); ++iter){
		(*iter)->step();
	}
	if(m_hero)
		m_hero->step();
}

// todo:
int SceneLayer::handleEvent(eActionEventType event, int x, int y)
{
	switch(event)
	{
	case eRawActionDown:
		break;
	case eRawActionUp:
		m_hero->startWaySearch(x+Camera::Instance()->getCameraX(), y+Camera::Instance()->getCameraY());
		break;
	}
	return -1;
}

void SceneLayer::drawImp()
{
	// terrain
	if( m_terrain){
		Render::Instance()->drawTexture(m_terrain, 
			-Camera::Instance()->getCameraX(), 
			-Camera::Instance()->getCameraY());
		Render::Instance()->drawTexture(m_terrain, 
			-Camera::Instance()->getCameraX()+m_terrain->m_imageWidth, 
			-Camera::Instance()->getCameraY());
	}

	//actors
	vector<Actor * >::iterator iter = m_actors.begin();
	for(; iter != m_actors.end(); ++iter){
		(*iter)->paint();
	}

	if(m_hero){
		m_hero->paint();
	}
}


void SceneLayer::loadScene(unsigned int key)
{
	releaseScene();
	
	LOGD("%s","loadScene Begin");

	loadHero();
	Camera::Instance()->setCamera(m_hero);

	LOGD("%s", "loadScene Ok");
}


void SceneLayer::loadHero()
{
	if(!m_hero){
		m_hero = 	new Hero();
		LOGD("%s", "load hero sprite");
		m_hero->setAction(0, TRUE);
		m_hero->setPosition(Render::Instance()->getScreenWidth()/2, Render::Instance()->getScreenHeight()/2);
		LOGD("%s", "load hero");
	}
}

void SceneLayer::releaseScene()
{
	m_touchActor = NULL;
	SAFE_RELEASE(m_terrain);
	SAFE_FREE(m_collides);
}
/********************************************************************
	created:	2012/10/23
	filename: 	SceneLayer.h
	author:		reedhong
	
	purpose:	场景层，存储场景，场景中的人物，还有碰撞层等的信息
*********************************************************************/

#ifndef __SceneLayer_H__
#define __SceneLayer_H__

#include <vector>
using namespace std;

#include "RenderSystem/Texture.h"
#include "RenderSystem/Sprite.h"
using namespace Lotus2d;

#include "AbstractLayer.h"
#include "Actor/Actor.h"
#include "Actor/Hero.h"

class SceneLayer: public AbstractLayer
{
public:
	SceneLayer();
	virtual ~SceneLayer();

	virtual void enter();

	virtual void leave();

	virtual void step();

	// todo:
	virtual int handleEvent(eActionEventType event, int x, int y);

	virtual void drawImp();
	void loadScene(unsigned int key);
private:
	// 清除场景中的数据
	void releaseScene();
	
	void loadHero();
	Actor* findActor(int touchX, int touchY);
private:

	
	// scene info
	int				m_width;
	int				m_height;
	Texture*		m_terrain;
	vector<Actor* >	m_actors;
	Actor*		m_touchActor;

	// 碰撞信息
	char*			m_collides;
	int				m_gridWidthCount;
	int				m_gridHeightCount;
	int				m_gridCount;

	//hero
	Hero*			m_hero;
};


#endif
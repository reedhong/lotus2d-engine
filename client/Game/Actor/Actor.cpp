/********************************************************************
	created:	2012/10/24
	filename: 	Actor.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/
#include "RenderSystem/Render.h"
#include "Base/Util.h"
using namespace Lotus2d;

#include "Actor.h"
#include "Scene/Camera.h"
#include "Constants/Constants.h"
#include "Constants/GameConfig.h"




static int calcDirection(int32 x1,int32 y1,int32 x2,int32 y2)
{
	int32 direction = DIRECT_UP;
	if(x1==x2&&y2>y1)
	{
		direction = DIRECT_DOWN;
	}
	else if(x1==x2&&y2<y1)
	{
		direction = DIRECT_UP;
	}
	else if(x1>x2&&y2==y1)
	{
		direction = DIRECT_LEFT;
	}
	else if(x1<x2&&y2==y1)
	{
		direction = DIRECT_RIGHT;
	}
	else if(x1<x2&&y2<y1)
	{
		direction = DIRECT_RIGHT_UP;
	}
	else if(x1<x2&&y2>y1)
	{
		direction = DIRECT_RIGHT_DOWN;
	}
	else if(x1>x2&&y2<y1)
	{
		direction = DIRECT_LEFT_UP;
	}
	else if(x1>x2&&y2>y1)
	{
		direction = DIRECT_LEFT_DOWN;
	}
	return direction;
}

static void calWalkingSpeed(int dir, int *x, int *y)
{
	
}

Actor::	Actor()
{
	m_sprite = NULL;
	m_x = 0;
	m_y = 0;
	m_name  = "";
	m_sprite = NULL;	
	m_isWalking = FALSE;
	m_flags = 0;

	m_action = 0;
	m_frameIndex = 0;
	m_frameDuration = 0;
}

Actor::Actor(const string& name, int x, int y)
{
	m_sprite = NULL;
	setPosition(x,  y);
	m_name  = name;

	m_action = 0;
	m_frameIndex = 0;
	m_frameDuration = 0;

	m_isWalking = FALSE;
	m_flags = 0;
}

void Actor::setSprite(Sprite* sprite)
{
	m_sprite = sprite;
	sprite->addRef();
}

Actor::~Actor(void)
{
	SAFE_RELEASE(m_sprite);

}

void Actor::setAction(int  action,  BOOL mustChanged)
{
	int newActionID = checkNewAction(action);
	if(m_sprite == NULL)
		return;
	if (!mustChanged && m_action == newActionID)
		return;
	//做一个校验，如果溢出，则默认为0	
	m_action = newActionID;
	m_frameIndex = 0;
	m_frameDuration = 0;
	removeFlags(FLAG_ACTION_END);
}
int Actor::checkNewAction(int action)
{
	return action;
	
}

BOOL Actor::IsSelected(int x, int y)
{
	return Util::isInRect(x, y, 
		m_x-Camera::Instance()->getCameraX()+m_sprite->m_activeZone.x,
		m_y-Camera::Instance()->getCameraY()+m_sprite->m_activeZone.y,
		m_sprite->m_activeZone.w, 
		m_sprite->m_activeZone.h);
}

void Actor::paint()
{
	if(NULL == m_sprite )
		return ;

	//m_sprite->setAction(m_action);   // 由于sprite是重用的，所以这个地方需要每次重新设置
	m_sprite->drawAction(m_action, m_frameIndex,
		m_x-Camera::Instance()->getCameraX(), 
		m_y-Camera::Instance()->getCameraY(), hasFlags(FLAG_FLIPX), hasFlags(FLAG_FLIPY));
	
	
}

void Actor::step()
{
	GotoNextFrame();
	if(m_isWalking){
		goWay();
	}
}

void Actor::GotoNextFrame()
{
	m_frameDuration++;
	SpriteAction* action = m_sprite->m_actions[m_action];
	SpriteAFrame* aframe = action->m_aframes[m_frameIndex];
	if(aframe->m_times == m_frameDuration){ // frame绘制结束
		m_frameIndex++;
		m_frameIndex%=action->m_aframes.size();
		m_frameDuration = 0;
	}
}

void Actor::paintZone()
{
	Render::Instance()->drawRect(
		m_x-Render::Instance()->getCameraX()+m_sprite->m_activeZone.x, 
		m_y-Render::Instance()->getCameraY()+m_sprite->m_activeZone.y,
		m_sprite->m_activeZone.w, 
		m_sprite->m_activeZone.h, 0xFFFF0000);
}


void Actor::setPosition(int x, int y)
{
	m_x = x;
	m_y = y;
	//m_gridX = x >> COLLIDE_GRID_SHIFT_W;
	//m_gridY = y >> COLLIDE_GRID_SHIFT_H;
}

void Actor::setDirection()
{
	switch (m_direction) {
		case DIRECT_LEFT:
			addFlags(FLAG_FLIPX);
			break;
		case DIRECT_UP:
		case DIRECT_DOWN:
		case DIRECT_RIGHT:
			removeFlags(FLAG_FLIPX);
			break;
	}
}

void Actor::walk()
{
	setDirection();
	removeFlags(FLAG_STAND);
	int8 offset = 0;
	switch (m_direction) {
		case DIRECT_LEFT:
		case DIRECT_LEFT_DOWN:
		case DIRECT_LEFT_UP:
			addFlags(FLAG_FLIPX);
			break;
		case DIRECT_RIGHT:
		case DIRECT_RIGHT_DOWN:
		case DIRECT_RIGHT_UP:
			removeFlags(FLAG_FLIPX);
			break;
		case DIRECT_DOWN:
			offset = 1;
			break;
		case DIRECT_UP:
			offset = 2;
			break;
		default:
			break;
	}
	setAction(ACTION_XINGZOU+offset, FALSE);
}

void Actor::stand()
{
	setDirection();
	addFlags(FLAG_STAND);

	int8 offset = 0;

	switch (m_direction) {
		case DIRECT_LEFT:
		case DIRECT_LEFT_DOWN:
		case DIRECT_LEFT_UP:
			addFlags(FLAG_FLIPX);
			break;
		case DIRECT_RIGHT:
		case DIRECT_RIGHT_DOWN:
		case DIRECT_RIGHT_UP:
			removeFlags(FLAG_FLIPX);
			break;
		case DIRECT_DOWN:
			offset = 1;
			break;
		case DIRECT_UP:
			offset = 2;
			break;
	}

	setAction(ACTION_ZHANLI+offset, FALSE);
}

void Actor::startWaySearch(int x, int y)
{
	
}

void Actor::endWay()
{
	m_isWalking = FALSE;
	stand();
}

void Actor::goWay()
{
	

}
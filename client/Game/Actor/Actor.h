/********************************************************************
	created:	2012/10/24
	filename: 	Actor.h
	author:		reedhong
	
	purpose:	场景一般Actor
*********************************************************************/
#ifndef	__Actor_H__
#define	__Actor_H__

#include <string>
using namespace std;

#include "RenderSystem/Sprite.h"
#include "Base/ObjectBase.h"
using namespace Lotus2d;

#define DIRECT_UP			8
#define DIRECT_DOWN			2
#define DIRECT_RIGHT		4
#define DIRECT_LEFT			1
#define DIRECT_LEFT_UP		(DIRECT_LEFT|DIRECT_UP)
#define DIRECT_LEFT_DOWN	(DIRECT_LEFT|DIRECT_DOWN)
#define DIRECT_RIGHT_UP		(DIRECT_RIGHT|DIRECT_UP)
#define DIRECT_RIGHT_DOWN	(DIRECT_RIGHT|DIRECT_DOWN)

class Actor: public ObjectBase
{
protected:
	Actor(const Actor& actor){}
	Actor& operator =(const Actor& a) {}
public:
	Actor();
	Actor(const string& name, int x=0, int y=0);	
	~Actor(void);
	void paint();
	void paintZone();

	void setAction(int  action,  BOOL mustChanged);
	int checkNewAction(int action);

	void step();

	void setName(const string& name){
		m_name = name;
	}
	BOOL IsSelected(int x, int y);

	void setAction(unsigned int index);
	void GotoNextFrame();

	BOOL hasFlags(int flags) 
	{
		return (m_flags & flags) != 0;
	}


	void addFlags(int flags) 
	{
		m_flags |= flags;
	}
	void removeFlags(int flags) 
	{
		m_flags &= ~flags;
	}

	void startWaySearch(int x, int y);
	void goWay();
	void endWay();

	void setPosition(int x, int y);
	void setDirection();

	void walk();
	void stand();
	void setSprite(Sprite* sprite);
protected:
	Sprite*		m_sprite;
public:
	int				m_x;
	int				m_y;
	string			m_name;
	string			m_path;


	// 绘制相关, 记录当前正在绘制的action和frame
	int	m_action;	
	int	m_frameIndex;
	int	m_frameDuration;

	// 寻路
	int	m_destGridX;		// 目的各自
	int	m_destGridY;
	int	m_gridX;
	int	m_gridY;
	int	m_direction;
	BOOL m_isWalking;	// 标识Actor是否处于行走中

	// 标识
	int	m_flags;
};

#endif

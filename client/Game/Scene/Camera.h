/********************************************************************
	created:	2012/10/27
	filename: 	Camera.h
	author:		reedhong
	
	purpose:	Camera
*********************************************************************/

#ifndef __Game_Camera_H__
#define __Game_Camera_H__


#include "Actor/Actor.h"

enum eCameraType
{	
	eCameraFollowActor=0,
	eCameraFollowPoint,
};

class Camera
{
public:
	Camera();
	~Camera();
	void setCamera(int x, int y);
	void setCamera(Actor* actor);
	void init(int width, int height);
	int getCameraX() { return m_camX;}
	int getCameraY() { return m_camY;}
	void step();

	static Camera* Instance();
private:
	void validateCamara();
private:
	int		m_camX;
	int		m_camY;

	eCameraType  m_cameType;
	Actor* m_camActor;
	int m_camDestX ;
	int m_camDestY;

	int m_camMoveSpeed;

	int m_minCamX;
	int m_minCamY;
	int m_maxCamX;
	int m_maxCamY;


};


#endif
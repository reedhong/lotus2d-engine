/********************************************************************
	created:	2012/10/27
	filename: 	Camera.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "Camera.h"

#include "RenderSystem/Render.h"
using namespace Lotus2d;

static int limit(int value, int min, int max)
{
	if (value > max)
		return max;
	else if (value < min)
		return min;
	else
		return value;
}

Camera::Camera()
{
	m_camX = 0;
	m_camY = 0;

	m_camActor = NULL;
	m_camDestX  = 0;
	m_camDestY = 0;

	m_camMoveSpeed = 16;

	m_minCamX = 0;
	m_minCamY = 0;
	m_maxCamX = 0;
	m_maxCamY = 0;
}

Camera::~Camera()
{

}

Camera* Camera::Instance()
{
	static Camera s_instance;
	return &s_instance;
}

void Camera::setCamera(int x, int y)
{
	m_cameType = eCameraFollowPoint;
	m_camDestX = x;
	m_camDestY = y;
}

void Camera::setCamera(Actor* actor)
{
	m_cameType = eCameraFollowActor;
	m_camActor = actor;
}

void Camera::init(int width, int height)
{
	m_camX = m_camY = m_minCamX = m_minCamY = 0;
	m_maxCamX = width - Render::Instance()->getScreenWidth();
	m_maxCamY = height - Render::Instance()->getScreenHeight();
	if(m_maxCamY<0)
		m_maxCamY=0;
}

void Camera::step()
{
	static int desCamX;
	static int desCamY;
	if (m_cameType == eCameraFollowActor){
		if (m_camActor){
			desCamX = m_camActor->m_x - (Render::Instance()->getScreenWidth() >> 1);
			desCamY = m_camActor->m_y - (Render::Instance()->getScreenWidth() >> 1) - 30;

			m_camX = limit(desCamX, m_camX - m_camMoveSpeed, m_camX + m_camMoveSpeed);
			m_camY = limit(desCamY, m_camY - m_camMoveSpeed, m_camY + m_camMoveSpeed);
		}

	}else{
		m_camX = m_camDestX - ( Render::Instance()->getScreenWidth()>>1);
		m_camY = m_camDestY - ( Render::Instance()->getScreenHeight()>>1);
	}

	validateCamara();
	//LOGD("camX=%d, camY=%d", m_camX, m_camX);
}

void Camera::validateCamara()
{
	m_camX = limit(m_camX, m_minCamX, m_maxCamX);
	m_camY = limit(m_camY, m_minCamY, m_maxCamY);
}
/********************************************************************
	created:	2012/09/22
	filename: 	Particle.h
	author:		reedhong
	
	purpose:	粒子系统
*********************************************************************/

#ifndef __Lotus2d_Particle_H__
#define __Lotus2d_Particle_H__

#include "Math/Math.h"
#include "Math/Vector2.h"
#include "Base/ObjectBase.h"

namespace Lotus2d {
	class Particle: public ObjectBase
	{
	public:
		Vector2f     pos;
		Vector2f     startPos;

		Color4f	color;
		Color4f	deltaColor;

		float		size;
		float		deltaSize;

		float		rotation;
		float		deltaRotation;

		// 帧数
		int		timeToLive;

		//! Mode A: gravity, direction, radial accel, tangential accel
		struct {
			Vector2f		dir;
			float		radialAccel;
			float		tangentialAccel;
		} modeA;

		//! Mode B: radius mode
		struct {
			float		angle;
			float		degreesPerSecond;
			float		radius;
			float		deltaRadius;
		} modeB;

	};
}


#endif
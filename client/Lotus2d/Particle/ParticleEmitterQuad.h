/********************************************************************
created:	2012/10/31
filename: 	ParticleEmitterQuad.h
author:		reedhong

purpose:	四边形粒子发射器，区别于点粒子系统(GL_POINT_SPRITE_OES,GL_POINT_SIZE_ARRAY_OES)
					但cocos2d-x说，后者的效率在老的机器上高于四边形，但在新的机型上反而地，所以，点粒子系统
					不予实现，具体可以见cocos2d-x
*********************************************************************/

#ifndef __Lotus2d_PointParticleEmitter_H__
#define __Lotus2d_PointParticleEmitter_H__

#include "ParticleEmitter.h"

namespace Lotus2d {
	class ParticleEmitterQuad: public ParticleEmitter
	{
	public:
		ParticleEmitterQuad(){}
		virtual ~ParticleEmitterQuad();
		// super methods
		virtual bool initWithTotalParticles(unsigned int numberOfParticles);
		virtual void updateQuadWithParticle(Particle* particle, const Vector2f& newPosition);
		virtual void postStep();
		virtual void paintImp();
	private:
		void paintParticle(Particle* particle);
	};
}
#endif
/********************************************************************
	created:	2012/11/03
	filename: 	ParticleEmitterExamples.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "ParticleEmitterExamples.h"

namespace Lotus2d {
	ParticleEmitterSnow::ParticleEmitterSnow()
	{
		m_mode= eParticleModeGravity;

		// Gravity Mode: gravity
		m_modeA.gravity = Vector2f(0,0);

		// Gravity Mode: speed of particles
		m_modeA.speed = 10;
		m_modeA.speedVar = 1;

		// Gravity Mode: radial
		m_modeA.radialAccel = 0;
		m_modeA.radialAccelVar = 1;

		// Gravity mode: tagential
		m_modeA.tangentialAccel = 0;
		m_modeA.tangentialAccelVar = 1;

		// emitter position
		//CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		//this->setPosition(ccp(winSize.width/2, winSize.height + 10));
		//m_tPosVar = ccp( winSize.width/2, 0 );

		// angle
		m_angle = 90;
		m_angleVar = 5;

		// life of particles
		m_life = 60;
		m_lifeVar = 10;
		m_startFrame = 0;
		m_endFrame = 50;
		m_sourcePos.x = 480;
		m_sourcePos.y = 320;
		m_sourcePosVar.x = 480;

		// size, in pixels
		m_startSize = 0.8f;
		m_startSizeVar = 0.2f;
		//m_fEndSize = kCCParticleStartSizeEqualToEndSize;

		// emits per second
		m_emissionRate = 30;

		// color of particles
		m_startColor.r = 1.0f;
		m_startColor.g = 1.0f;
		m_startColor.b = 1.0f;
		m_startColor.a = 1.0f;
		m_startColorVar.r = 0.0f;
		m_startColorVar.g = 0.0f;
		m_startColorVar.b = 0.0f;
		m_startColorVar.a = 0.0f;
		m_endColor.r = 0.5f;
		m_endColor.g = 0.5f;
		m_endColor.b = 0.5f;
		m_endColor.a = 1.0f;
		m_endColorVar.r = 0.0f;
		m_endColorVar.g = 0.0f;
		m_endColorVar.b = 0.0f;
		m_endColorVar.a = 0.0f;

		setTexture("X:\\particle\\snow.png");

	}

	bool ParticleEmitterSnow::initWithTotalParticles(unsigned int numberOfParticles)
	{
		ParticleEmitterQuad::initWithTotalParticles(numberOfParticles);
		// duration
		//m_duration = eParticleDurationInfinity;
		// set gravity mode.
	
		return true;
	}
}

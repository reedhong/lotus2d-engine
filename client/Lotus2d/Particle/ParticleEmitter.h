/********************************************************************
created:	2012/10/30
filename: 	ParticleEmitter.h
author:		reedhong

purpose:	粒子发射器
*********************************************************************/

#ifndef __Lotus2d_ParticleEmitter_H__
#define __Lotus2d_ParticleEmitter_H__

#include "Math/Math.h"
#include "Math/Vector2.h"
#include "Base/Types.h"

#include "Particle.h"

#include "RenderSystem/Texture.h"
#include "Base/ObjectBase.h"

namespace Lotus2d {
		//* @enum
	enum {
		/** The Particle emitter lives forever */
		eParticleDurationInfinity = -1,

		/** The starting size of the particle is equal to the ending size */
		eParticleStartSizeEqualToEndSize = -1,

		/** The starting radius of the particle is equal to the ending radius */
		eParticleStartRadiusEqualToEndRadius = -1,

		// backward compatible
		//eParticleStartSizeEqualToEndSize = eParticleStartSizeEqualToEndSize,
		//eParticleDurationInfinity = eParticleDurationInfinity,
	};

	//* @enum
	enum {
		/** Gravity mode (A mode) */
		eParticleModeGravity,

		/** Radius mode (B mode) */
		eParticleModeRadius,	
	};


	/** @typedef tCCPositionType
	possible types of particle positions
	*/
	typedef enum {
		/** Living particles are attached to the world and are unaffected by emitter repositioning. */
		ePositionTypeFree,

		/** Living particles are attached to the world but will follow the emitter repositioning.
		Use case: Attach an emitter to an sprite, and you want that the emitter follows the sprite.
		*/
		ePositionTypeRelative,

		/** Living particles are attached to the emitter and are translated along with it. */
		ePositionTypeGrouped,
	}ePositionType;

	//! Blend Function used for textures
	typedef struct _BlendFunc
	{
		//! source blend function
		int src;
		//! destination blend function
		int dst;
	} BlendFunc;

	class ParticleEmitter : public ObjectBase
	{
	public:
		ParticleEmitter();
		virtual ~ParticleEmitter();
		virtual void updateQuadWithParticle(Particle* particle, const Vector2f& newPosition)=0;
		virtual void paintImp() = 0;
		virtual bool initWithTotalParticles(unsigned int numberOfParticles);
	public:
		void paint();
		void stop();
		void reset();
		void step();
		virtual void postStep();
		void setTexture(Texture* texture);
		void setTexture(const char* path);
		void changeActive()
		{
			m_isActive = !m_isActive;
		}
	protected:
		bool addParticle();
		bool isFull();
		void initParticle(Particle* particle);
	public:
		const static  int PARTICLE_DURATION_INFINITY = -1;  
		const static  int PARTICLE_START_SIZE_EQUAL_TO_END_SIZE = -1;  
		const static  int PARTICLE_START_RADIUS_EQUAL_TO_END_RADIUS = -1;  

		std::string m_sPlistFile;
		std::string m_name;
		std::string m_texturePath;
		Vector2f		m_position;
		struct {
			/** Gravity value. Only available in 'Gravity' mode. */
			Vector2f gravity;
			/** speed of each particle. Only available in 'Gravity' mode.  */
			float speed;
			/** speed variance of each particle. Only available in 'Gravity' mode. */
			float speedVar;
			/** tangential acceleration of each particle. Only available in 'Gravity' mode. */
			float tangentialAccel;
			/** tangential acceleration variance of each particle. Only available in 'Gravity' mode. */
			float tangentialAccelVar;
			/** radial acceleration of each particle. Only available in 'Gravity' mode. */
			float radialAccel;
			/** radial acceleration variance of each particle. Only available in 'Gravity' mode. */
			float radialAccelVar;
		} m_modeA;

		//! Mode B: circular movement (gravity, radial accel and tangential accel don't are not used in this mode)
		struct {
			/** The starting radius of the particles. Only available in 'Radius' mode. */
			float startRadius;
			/** The starting radius variance of the particles. Only available in 'Radius' mode. */
			float startRadiusVar;
			/** The ending radius of the particles. Only available in 'Radius' mode. */
			float endRadius;
			/** The ending radius variance of the particles. Only available in 'Radius' mode. */
			float endRadiusVar;			
			/** Number of degress to rotate a particle around the source pos per second. Only available in 'Radius' mode. */
			float rotatePerSecond;
			/** Variance in degrees for rotatePerSecond. Only available in 'Radius' mode. */
			float rotatePerSecondVar;
		} m_modeB;

		//! Array of particles
		Particle *m_particles;

		// color modulate
		//	BOOL colorModulate;

		//! How many particles can be emitted per second
		float m_emitCounter;

		//!  particle idx
		unsigned int m_particleIdx;

		/** Is the emitter active */
		bool  m_isActive;
		/** Quantity of particles that are being simulated at the moment */
		unsigned int m_particleCount;

		/** sourcePosition of the emitter */
		Vector2f m_sourcePos;
		/** Position variance of the emitter */
		Vector2f m_sourcePosVar;
		/** life, and life variation of each particle */
		int m_life;
		/** life variance of each particle */
		int m_lifeVar;
		/** angle and angle variation of each particle */
		float m_angle;
		/** angle variance of each particle */
		float m_angleVar;

		/** start size in pixels of each particle */
		float m_startSize;
		/** size variance in pixels of each particle */
		float m_startSizeVar;
		/** end size in pixels of each particle */
		float m_endSize;
		/** end size variance in pixels of each particle */
		float m_endSizeVar;
		/** start color of each particle */
		Color4f m_startColor;
		/** start color variance of each particle */
		Color4f m_startColorVar;
		/** end color and end color variation of each particle */
		Color4f m_endColor;
		/** end color variance of each particle */
		Color4f m_endColorVar;
		//* initial angle of each particle
		float m_startSpin;
		//* initial angle of each particle
		float m_startSpinVar;
		//* initial angle of each particle
		float  m_endSpin;
		//* initial angle of each particle
		float  m_endSpinVar;
		/** emission rate of the particles */
		float  m_emissionRate;
		/** maximum particles of the system */
		unsigned int  m_totalParticles;
		/** conforms to CocosNodeTexture protocol */
		Texture* m_texture;
		/** conforms to CocosNodeTexture protocol */
		BlendFunc m_blendFunc;
		/** whether or not the particles are using blend additive.
		If enabled, the following blending function will be used.
		@code
		source blend function = GL_SRC_ALPHA;
		dest blend function = GL_ONE;
		@endcode
		*/
		bool m_isBlendAdditive;
		/** particles movement type: Free or Grouped
		@since v0.8
		*/
		ePositionType  m_positionType;
		/* 是否重复*/
		bool m_isRepeat;
		/** Switch between different kind of emitter modes:
		- kCCParticleModeGravity: uses gravity, speed, radial and tangential acceleration
		- kCCParticleModeRadius: uses radius movement + rotation
		*/
		int m_mode;

		int	m_endFrame;  // 播放到第几帧结束
		int	m_startFrame;	// 从第几帧开始播
		int	m_currentFrame; // 当前是第几帧
	};
}

#endif
/********************************************************************
	created:	2012/10/30
	filename: 	ParticleEmitter.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "ParticleEmitter.h"
#include "Base/Macros.h"
#include "Base/Log.h"
#include "RenderSystem/Render.h"

namespace Lotus2d{

	ParticleEmitter::ParticleEmitter()
		:m_sPlistFile("")
		,m_particles(NULL)
		,m_emitCounter(0)
		,m_particleIdx(0)
		,m_isActive(true)
		,m_particleCount(0)
		,m_life(0)
		,m_lifeVar(0)
		,m_angle(0)
		,m_angleVar(0)
		,m_startSize(1.0f)
		,m_startSizeVar(0)
		,m_endSize(eParticleStartSizeEqualToEndSize)
		,m_endSizeVar(0)
		,m_startSpin(0)
		,m_startSpinVar(0)
		,m_endSpin(0)
		,m_endSpinVar(0)
		,m_emissionRate(0)
		,m_totalParticles(0)
		,m_texture(NULL)
		,m_isBlendAdditive(false)
		,m_positionType(ePositionTypeFree)
		,m_isRepeat(false)
		,m_mode(eParticleModeGravity)
		,m_endFrame(0)
		,m_startFrame(0)
		,m_currentFrame(0)
	{
		m_modeA.gravity = Vector2f::ZERO;
		m_modeA.speed = 0;
		m_modeA.speedVar = 0;
		m_modeA.tangentialAccel = 0;
		m_modeA.tangentialAccelVar = 0;
		m_modeA.radialAccel = 0;
		m_modeA.radialAccelVar = 0;
		m_modeB.startRadius = 0;
		m_modeB.startRadiusVar = 0;
		m_modeB.endRadius = 0;
		m_modeB.endRadiusVar = 0;			
		m_modeB.rotatePerSecond = 0;
		m_modeB.rotatePerSecondVar = 0;
		// todo
		//m_tBlendFunc.src = CC_BLEND_SRC;
		//m_tBlendFunc.dst = CC_BLEND_DST;
	}


	bool ParticleEmitter::initWithTotalParticles(unsigned int numberOfParticles)
	{
		if(m_totalParticles == numberOfParticles)
			return  true;
		m_totalParticles = numberOfParticles;

		SAFE_DELETE_ARRAY(m_particles);

		m_particles = new Particle[m_totalParticles];

		if( ! m_particles )
		{
			LOGD("%s","Particle system: not enough memory");
			return false;
		}

		// default, active
		m_isActive = true;

		// default blend function
		//m_tBlendFunc.src = CC_BLEND_SRC;
		//m_tBlendFunc.dst = CC_BLEND_DST;

		// default movement type;
		m_positionType = ePositionTypeFree;

		// by default be in mode A:
		m_mode = eParticleModeGravity;

		// default: modulate
		// XXX: not used
		//	colorModulate = YES;

		m_isRepeat = FALSE;

		// todo:
		if(m_life > 0){
			m_emissionRate = ((float)m_totalParticles)/m_life;
		}else{
			m_emissionRate = 0;
		}
		return true;
	}

	ParticleEmitter::~ParticleEmitter()
	{
		SAFE_DELETE_ARRAY(m_particles);
		SAFE_RELEASE(m_texture)
	}
	
	bool ParticleEmitter::addParticle()
	{
		if (this->isFull())
		{
			return false;
		}

		Particle * particle = &m_particles[ m_particleCount ];
		this->initParticle(particle);
		++m_particleCount;

		return true;
	}
	void ParticleEmitter::initParticle(Particle* particle)
	{
		// timeToLive
		// no negative life. prevent division by 0
		particle->timeToLive = m_life + (int)(m_lifeVar * RANDOM_MINUS1_1());
		particle->timeToLive = MAX(0, particle->timeToLive);

		// position
		particle->pos.x = m_sourcePos.x + m_sourcePosVar.x * RANDOM_MINUS1_1();
		particle->pos.x *= GET_SCALE_FACTOR();
		particle->pos.y = m_sourcePos.y + m_sourcePosVar.y * RANDOM_MINUS1_1();
		particle->pos.y *= GET_SCALE_FACTOR();

		// Color
		Color4f start;
		start.r = Math::clampf(m_startColor.r + m_startColorVar.r * RANDOM_MINUS1_1(), 0, 1);
		start.g = Math::clampf(m_startColor.g + m_startColorVar.g * RANDOM_MINUS1_1(), 0, 1);
		start.b = Math::clampf(m_startColor.b + m_startColorVar.b * RANDOM_MINUS1_1(), 0, 1);
		start.a = Math::clampf(m_startColor.a + m_startColorVar.a * RANDOM_MINUS1_1(), 0, 1);

		Color4f end;
		end.r = Math::clampf(m_endColor.r + m_endColorVar.r * RANDOM_MINUS1_1(), 0, 1);
		end.g = Math::clampf(m_endColor.g + m_endColorVar.g * RANDOM_MINUS1_1(), 0, 1);
		end.b = Math::clampf(m_endColor.b + m_endColorVar.b * RANDOM_MINUS1_1(), 0, 1);
		end.a = Math::clampf(m_endColor.a + m_endColorVar.a * RANDOM_MINUS1_1(), 0, 1);

		particle->color = start;
		particle->deltaColor.r = (end.r - start.r) / particle->timeToLive;
		particle->deltaColor.g = (end.g - start.g) / particle->timeToLive;
		particle->deltaColor.b = (end.b - start.b) / particle->timeToLive;
		particle->deltaColor.a = (end.a - start.a) / particle->timeToLive;

		// size
		float startS = m_startSize + m_startSizeVar * RANDOM_MINUS1_1();
		startS = MAX(0, startS); // No negative value
		startS *= GET_SCALE_FACTOR();

		particle->size = startS;

		if( m_endSize == eParticleStartSizeEqualToEndSize )
		{
			particle->deltaSize = 0;
		}
		else
		{
			float endS = m_endSize + m_endSizeVar * RANDOM_MINUS1_1();
			endS = MAX(0, endS); // No negative values
			endS *= GET_SCALE_FACTOR();
			particle->deltaSize = (endS - startS) / particle->timeToLive;
		}

		// rotation
		float startA = m_startSpin + m_startSpinVar * RANDOM_MINUS1_1();
		float endA = m_endSpin + m_endSpinVar * RANDOM_MINUS1_1();
		particle->rotation = startA;
		particle->deltaRotation = (endA - startA) / particle->timeToLive;

		// position
		if( m_positionType == ePositionTypeFree )
		{
			//Vector2f p = this->convertToWorldSpace(CCPointZero);
			//particle->startPos = ccpMult( p, CC_CONTENT_SCALE_FACTOR() );
		}
		else if ( m_positionType == ePositionTypeRelative )
		{
			//particle->startPos = ccpMult( m_tPosition, CC_CONTENT_SCALE_FACTOR() );
		}

		// direction
		float a = DEGREES_TO_RADIANS( m_angle + m_angleVar * RANDOM_MINUS1_1() );	

		// Mode Gravity: A
		if( m_mode == eParticleModeGravity ) 
		{
			Vector2f v(cosf( a ), sinf( a ));
			float s = m_modeA.speed + m_modeA.speedVar * RANDOM_MINUS1_1();
			s *= GET_SCALE_FACTOR();

			// direction
			particle->modeA.dir = ( v *s );

			// radial accel
			particle->modeA.radialAccel = m_modeA.radialAccel + m_modeA.radialAccelVar * RANDOM_MINUS1_1();
			particle->modeA.radialAccel *= GET_SCALE_FACTOR();

			// tangential accel
			particle->modeA.tangentialAccel = m_modeA.tangentialAccel + m_modeA.tangentialAccelVar * RANDOM_MINUS1_1();
			particle->modeA.tangentialAccel *= GET_SCALE_FACTOR();
		}

		// Mode Radius: B
		else {
			// Set the default diameter of the particle from the source position
			float startRadius = m_modeB.startRadius + m_modeB.startRadiusVar * RANDOM_MINUS1_1();
			float endRadius = m_modeB.endRadius + m_modeB.endRadiusVar * RANDOM_MINUS1_1();
			startRadius *= GET_SCALE_FACTOR();
			endRadius *= GET_SCALE_FACTOR();

			particle->modeB.radius = startRadius;

			if( m_modeB.endRadius == eParticleStartRadiusEqualToEndRadius )
				particle->modeB.deltaRadius = 0;
			else
				particle->modeB.deltaRadius = (endRadius - startRadius) / particle->timeToLive;

			particle->modeB.angle = a;
			particle->modeB.degreesPerSecond = DEGREES_TO_RADIANS(m_modeB.rotatePerSecond + m_modeB.rotatePerSecondVar *RANDOM_MINUS1_1());
		}	
	}
	void ParticleEmitter::stop()
	{
		m_isActive = false;
		m_currentFrame = m_endFrame;
		m_emitCounter = 0;
	}
	void ParticleEmitter::reset()
	{
		m_isActive = true;
		m_currentFrame = 0;
		for (m_particleIdx = 0; m_particleIdx < m_totalParticles; ++m_particleIdx)
		{
			Particle *p = &m_particles[m_particleIdx];
			p->timeToLive = 0;
		}
		m_particleCount = 0;
	}

	bool ParticleEmitter::isFull()
	{
		return (m_particleCount == m_totalParticles);
	}

	// ParticleSystem - MainLoop
	void ParticleEmitter::step()
	{
		if(!m_isActive)
			return ;
	
		m_currentFrame++;
		if(m_currentFrame < m_startFrame)
			return ;
		
		if(m_currentFrame > m_endFrame)
			return;

		int addNum=0;
		LOGD("%s","***********************step******************");
		LOGD("m_particleCount %d",m_particleCount);
		int count = (int)(m_emissionRate* RANDOM_0_1());
		while( m_particleCount < m_totalParticles && addNum < count){
			this->addParticle();
			addNum++;
		}

		if(m_currentFrame >= m_endFrame){
			m_currentFrame = 0;
		}
		
#if 0
		for(int i = 0; i < m_particleCount; i++){
			Particle *p = &m_particles[i];
			p->pos.x = 480+480*RANDOM_MINUS1_1();
			p->pos.y = 320+320*RANDOM_MINUS1_1();
		}
		return ;
#endif

		m_particleIdx = 0;


//#if CC_ENABLE_PROFILERS
		/// @todo CCProfilingBeginTimingBlock(_profilingTimer);
//#endif


		Vector2f currentPosition = Vector2f::ZERO;
		if( m_positionType == ePositionTypeFree )
		{
			//currentPosition = this->convertToWorldSpace(CCPointZero);
			//currentPosition.x *= CC_CONTENT_SCALE_FACTOR();
			//currentPosition.y *= CC_CONTENT_SCALE_FACTOR();
		}
		else if ( m_positionType == ePositionTypeRelative )
		{
			currentPosition = m_position;
			currentPosition.x *= GET_SCALE_FACTOR();
			currentPosition.y *= GET_SCALE_FACTOR();
		}
	
		while( m_particleIdx < m_particleCount )
		{
			Particle *p = &m_particles[m_particleIdx];
			//LOGD("begin particle %d, life=%d, pos =(%f,%f)", m_particleIdx, p->timeToLive, 
			//	p->pos.x, p->pos.y);
			// life
			p->timeToLive--;

			if( p->timeToLive > 0 ) 
			{
				// Mode A: gravity, direction, tangential accel & radial accel
				if( m_mode == eParticleModeGravity ) 
				{
					Vector2f tmp, radial, tangential;

					radial = Vector2f::ZERO;
					// radial acceleration
					if(p->pos.x || p->pos.y)
						radial = p->pos.normalize();
					tangential = radial;
					radial = radial * p->modeA.radialAccel;

					// tangential acceleration
					float newy = tangential.x;
					tangential.x = -tangential.y;
					tangential.y = newy;
					tangential = tangential * p->modeA.tangentialAccel;

					// (gravity + radial + tangential) * dt
					tmp =  radial + tangential + m_modeA.gravity;
					tmp = tmp * FRAME_TIME;
					p->modeA.dir = p->modeA.dir + tmp;
					tmp = p->modeA.dir * FRAME_TIME;
					p->pos = p->pos + tmp ;
				}

				// Mode B: radius movement
				else {				
					// Update the angle and radius of the particle.
					p->modeB.angle += p->modeB.degreesPerSecond*FRAME_TIME;
					p->modeB.radius += p->modeB.deltaRadius*FRAME_TIME;

					p->pos.x = - cosf(p->modeB.angle) * p->modeB.radius;
					p->pos.y = - sinf(p->modeB.angle) * p->modeB.radius;
				}

				// color
				p->color.r += p->deltaColor.r*FRAME_TIME;
				p->color.g += p->deltaColor.g*FRAME_TIME;
				p->color.b += p->deltaColor.b*FRAME_TIME;
				p->color.a += p->deltaColor.a*FRAME_TIME;

				// size
				p->size += p->deltaSize;
				p->size = MAX( 0, p->size );

				// angle
				p->rotation += p->deltaRotation;

				//
				// update values in quad
				//

				Vector2f	newPos;

				if( m_positionType == ePositionTypeFree || m_positionType == ePositionTypeRelative ) 
				{
					Vector2f diff = currentPosition - p->startPos ;
					newPos =p->pos - diff;
				} 
				else
				{
					newPos = p->pos;
				}

				updateQuadWithParticle(p, newPos);
				//updateParticleImp(self, updateParticleSel, p, newPos);

				// update particle counter
				//LOGD("after particle %d, life=%d, pos =(%f,%f)", m_particleIdx, p->timeToLive, 
				//	p->pos.x, p->pos.y);
				++m_particleIdx;

			} 
			else 
			{
				// life < 0
				if( m_particleIdx != m_particleCount-1 )
				{
					m_particles[m_particleIdx] = m_particles[m_particleCount-1];
				}
				--m_particleCount;

				if( m_particleCount == 0 && !m_isRepeat )
				{
					//this->unscheduleUpdate();
					//m_pParent->removeChild(this, true);
					return;
				}
			}
		}


		//#ifdef CC_USES_VBO
		this->postStep();
		//#endif
	}
	void ParticleEmitter::updateQuadWithParticle(Particle* particle, const Vector2f& newPosition)
	{
		UNUSED_PARAM(particle);
		UNUSED_PARAM(newPosition);
		// should be overriden
	}
	void ParticleEmitter::postStep()
	{
		// should be overriden
	}

	void ParticleEmitter::setTexture(Texture* texture)
	{
			SAFE_RELEASE(m_texture);
			m_texture = texture;
	}
	void ParticleEmitter::setTexture(const char* path)
	{
		Texture* texture = Texture::load(path);
		ASSERT(texture!=NULL);
		m_texturePath = path;
		setTexture(texture);
	}

	void ParticleEmitter::paint()
	{
		paintImp();
	}
}

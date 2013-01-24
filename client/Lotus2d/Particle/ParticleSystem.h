/********************************************************************
	created:	2012/10/30
	filename: 	ParticleSystem.h
	author:		reedhong
	
	purpose:	发射器的组合
*********************************************************************/

#ifndef __Lotus2d_ParticleSystem_H__
#define __Lotus2d_ParticleSystem_H__

#include "ParticleEmitter.h"
#include "Base/Types.h"

namespace Lotus2d {
	class ParticleSystem
	{
	public:
		ParticleSystem();
		~ParticleSystem();
	public:
		void addEmitter(ParticleEmitter* emitter);
		void delEmitter(unsigned int index);
		void paint();
		void step();
		void reset();
		ParticleEmitter* getEmitter(unsigned int index);
		unsigned int getEmitterSize(){ return m_emitters.size();}
		void resetFrameCount();
		void setActive(bool b) 
		{
			m_isActive = true;
		}

		void changeActive()
		{
			m_isActive = !m_isActive;
		}
	private:
		bool	m_isActive;
		vector<ParticleEmitter* > m_emitters;
		int	m_frameCount;	// 总帧数
		int	m_currentFrame; 

	};
}

#endif

/********************************************************************
	created:	2012/10/30
	filename: 	ParticleSystem.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "ParticleSystem.h"
#include "Base/Assert.h"

#include "RenderSystem/Render.h"

namespace Lotus2d {
	ParticleSystem::ParticleSystem()
	{
		m_frameCount = 0;
		m_currentFrame = 0;
		m_isActive = true;
	}

	ParticleSystem::~ParticleSystem()
	{
		vector<ParticleEmitter*>::iterator iter = m_emitters.begin();
		for(; iter != m_emitters.end(); ++iter){
			delete (*iter);
		}
		m_emitters.clear();
	}
	
	void ParticleSystem::reset()
	{
		m_isActive = true;
		vector<ParticleEmitter*>::iterator iter = m_emitters.begin();
		for(; iter != m_emitters.end(); ++iter){
			(*iter)->reset();
		}
	}

	ParticleEmitter* ParticleSystem::getEmitter(unsigned int index)
	{
		ASSERT(index < m_emitters.size());
		return m_emitters[index];
	}

	void ParticleSystem::addEmitter(ParticleEmitter* emitter)
	{
		m_emitters.push_back(emitter);
		resetFrameCount();
	}

	void ParticleSystem::resetFrameCount()
	{
		vector<ParticleEmitter*>::iterator iter = m_emitters.begin();
		for(; iter != m_emitters.end(); ++iter){
			if(m_frameCount < (*iter)->m_endFrame){
				m_frameCount = (*iter)->m_endFrame;
			}
		}
	}

	void ParticleSystem::delEmitter(unsigned int index)
	{
		vector<ParticleEmitter*>::iterator iter = m_emitters.begin();
		ParticleEmitter* emitter =  m_emitters[index];
		m_emitters.erase(iter+index);
		delete emitter;
	}

	void ParticleSystem::step()
	{
		if( !m_isActive)
			return ;
		m_currentFrame++;
		if(m_currentFrame >= m_frameCount){
			m_currentFrame = 0;
			//reset();
		}else{
			vector<ParticleEmitter*>::iterator iter = m_emitters.begin();
			for(; iter != m_emitters.end(); ++iter){
				(*iter)->step();
			}
		}
	}

	void ParticleSystem::paint()
	{
		Render::Instance()->enableVertexColor();
		vector<ParticleEmitter*>::iterator iter = m_emitters.begin();
		for(; iter != m_emitters.end(); ++iter){
			(*iter)->paint();
		}
		Render::Instance()->disableVertexColor();
	}
}
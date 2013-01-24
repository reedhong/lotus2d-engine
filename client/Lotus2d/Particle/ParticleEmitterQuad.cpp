/********************************************************************
created:	2012/10/31
filename: 	QuadParticleEmitter.cpp
author:		reedhong

purpose:	
*********************************************************************/

#include "ParticleEmitterQuad.h"
#include "RenderSystem/Render.h"

namespace Lotus2d
{
	bool ParticleEmitterQuad::initWithTotalParticles(unsigned int numberOfParticles)
	{
		// base initialization
		if( ParticleEmitter::initWithTotalParticles(numberOfParticles) ) 
		{
			return true;
		}
		return false;
	}
	ParticleEmitterQuad::~ParticleEmitterQuad()
	{
	}

	void ParticleEmitterQuad::updateQuadWithParticle(Particle* particle, const Vector2f& newPosition)
	{


#if 0
		// colors
		ccV2F_C4B_T2F_Quad *quad = &(m_pQuads[m_uParticleIdx]);

		Color4B color = {(GLubyte)(particle->color.r * 255), (GLubyte)(particle->color.g * 255), (GLubyte)(particle->color.b * 255), 
			(GLubyte)(particle->color.a * 255)};
		quad->bl.colors = color;
		quad->br.colors = color;
		quad->tl.colors = color;
		quad->tr.colors = color;

		// vertices
		GLfloat size_2 = particle->size/2;

		// 旋转
		if( particle->rotation ) 
		{
			GLfloat x1 = -size_2;
			GLfloat y1 = -size_2;

			GLfloat x2 = size_2;
			GLfloat y2 = size_2;
			GLfloat x = newPosition.x;
			GLfloat y = newPosition.y;

			GLfloat r = (GLfloat)-DEGREES_TO_RADIANS(particle->rotation);
			GLfloat cr = cosf(r);
			GLfloat sr = sinf(r);
			GLfloat ax = x1 * cr - y1 * sr + x;
			GLfloat ay = x1 * sr + y1 * cr + y;
			GLfloat bx = x2 * cr - y1 * sr + x;
			GLfloat by = x2 * sr + y1 * cr + y;
			GLfloat cx = x2 * cr - y2 * sr + x;
			GLfloat cy = x2 * sr + y2 * cr + y;
			GLfloat dx = x1 * cr - y2 * sr + x;
			GLfloat dy = x1 * sr + y2 * cr + y;

			// bottom-left
			quad->bl.vertices.x = ax;
			quad->bl.vertices.y = ay;

			// bottom-right vertex:
			quad->br.vertices.x = bx;
			quad->br.vertices.y = by;

			// top-left vertex:
			quad->tl.vertices.x = dx;
			quad->tl.vertices.y = dy;

			// top-right vertex:
			quad->tr.vertices.x = cx;
			quad->tr.vertices.y = cy;
		} else {
			// bottom-left vertex:
			quad->bl.vertices.x = newPosition.x - size_2;
			quad->bl.vertices.y = newPosition.y - size_2;

			// bottom-right vertex:
			quad->br.vertices.x = newPosition.x + size_2;
			quad->br.vertices.y = newPosition.y - size_2;

			// top-left vertex:
			quad->tl.vertices.x = newPosition.x - size_2;
			quad->tl.vertices.y = newPosition.y + size_2;

			// top-right vertex:
			quad->tr.vertices.x = newPosition.x + size_2;
			quad->tr.vertices.y = newPosition.y + size_2;				
		}
#endif
	}
	void ParticleEmitterQuad::postStep()
	{
#if 0
		glBindBuffer(GL_ARRAY_BUFFER, m_uQuadsID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_pQuads[0])*m_uParticleCount, m_pQuads);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif

	}

	// overriding draw method
	void ParticleEmitterQuad::paintImp()
	{	
		// 绘制粒子
		for(unsigned int i = 0; i < m_particleCount; i++){
			Particle* particle = &m_particles[i];
			paintParticle(particle);
		}
	}

	void ParticleEmitterQuad::paintParticle(Particle* particle)
	{
		int color = particle->color.getARGB();
		Render::Instance()->setVertexColor(color, color, color, color);
		Render::Instance()->drawTexture(m_texture, particle->pos.x, particle->pos.y, TRANS_NONE, ANCHOR_CENTER, particle->size);
		// todo: scale, rotation 没有考虑
	}
}
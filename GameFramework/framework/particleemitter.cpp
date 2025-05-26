// COMP710 GP Framework 2024

#include "particleemitter.h"

// Local includes:
#include "renderer.h"
#include "sprite.h"
#include "imgui.h"
#include "particle.h"
#include <iostream>
#include "inlinehelper.h"

ParticleEmitter::ParticleEmitter()
	: m_pSharedSprite(0)
	, m_particles(0)
	, m_fTimeElapsed(0)
	, m_iSpawnBatchSize(0)
	, m_fEmitRate(0)
	, m_fMaxLifeSpan(0)
	, m_fAccelerationScalar(0)
	, m_fColour{0}
	, m_fMinAngle(0)
	, m_fMaxAngle(0)
	, m_fX(0)
	, m_fY(0)
{

}

ParticleEmitter::~ParticleEmitter()
{
	delete m_pSharedSprite;
	m_pSharedSprite = 0;

	for (std::vector<Particle*>::iterator iter = m_particles.begin(); iter < m_particles.end(); ++iter)
	{
		delete* iter;
	}
	m_particles.clear();
}

bool ParticleEmitter::Initialize(Renderer& renderer)
{
	m_pSharedSprite = renderer.CreateSprite("sprites\\ball.png");
	return true;
}

void ParticleEmitter::Process(float deltaTime)
{
	if (m_particles.size() > 0)
	{
		for (std::vector<Particle*>::iterator iter = m_particles.begin(); iter < m_particles.end();)
		{
			if ((*iter)->m_IsAlive)
			{
				(*iter)->Process(deltaTime);
				++iter;
			}
			else
			{
				delete* iter;
				iter = m_particles.erase(iter);
			}
		}
	}
}

void ParticleEmitter::Draw(Renderer& renderer)
{
	for (std::vector<Particle*>::iterator iter = m_particles.begin(); iter < m_particles.end(); ++iter)
	{
		(*iter)->Draw(renderer);
	}
}

void ParticleEmitter::Spawn()
{
	for (int i = 0; i < m_iSpawnBatchSize; ++i)
	{
		// Emitter info
		m_fMaxLifeSpan = 2.5f;
		m_fAccelerationScalar = 15.0f;
		m_fColour[0] = 0.0f;
		m_fColour[1] = 255.0f;
		m_fColour[2] = 255.0f;
		m_fMinAngle = 0.0f;
		m_fMaxAngle = 90.0f;
		m_fX = 720.0f;
		m_fY = 450.0f;


		ParticleInfo info = { m_fX, m_fY, m_fMinAngle , m_fMaxAngle , m_fMaxLifeSpan, m_fAccelerationScalar };
		Particle* newParticle = new Particle();

		newParticle->SetParticleSprite(m_pSharedSprite);

		newParticle->Initialize(info);
		m_particles.push_back(newParticle);
	}
}

void ParticleEmitter::DebugDraw()
{
	ImGui::Text("Number of particles: %d", m_particles.size());
	ImGui::Text("Particles to Spawn:");
	ImGui::SliderInt("Num Particles", &m_iSpawnBatchSize, 0, 500);

}
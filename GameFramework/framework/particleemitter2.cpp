// COMP710 GP Framework 2024

#include "particleemitter2.h"

#include "renderer.h"
#include "sprite.h"
#include "imgui.h"
#include "particle2.h"

ParticleEmitter2::ParticleEmitter2()
	: m_pSharedSprite(0)
	, m_particles(0)
	, m_fTimeElapsed(0)
	, m_iSpawnBatchSize(0)
	, m_fEmitRate(0)
	, m_fMaxLifeSpan(0)
	, m_fAccelerationScalar(0)
	, m_fColor{ 0 }
	, m_fAngle(0)
	, m_fX(0)
	, m_fY(0)
{

}

ParticleEmitter2::~ParticleEmitter2()
{
	delete m_pSharedSprite;
	m_pSharedSprite = 0;

	for (std::vector<Particle2*>::iterator iter = m_particles.begin(); iter < m_particles.end(); ++iter)
	{
		delete* iter;
	}
	m_particles.clear();
}

bool ParticleEmitter2::Initialize(Renderer& renderer)
{
	m_pSharedSprite = renderer.CreateSprite("sprites\\ball.png");
	m_pSharedSprite->SetScale(0.01f);
	return true;
}

void ParticleEmitter2::Process(float deltaTime)
{
	for (std::vector<Particle2*>::iterator iter = m_particles.begin(); iter < m_particles.end();)
	{
		if ((*iter)->m_bAlive)
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

void ParticleEmitter2::Draw(Renderer& renderer)
{
	for (std::vector<Particle2*>::iterator iter = m_particles.begin(); iter < m_particles.end(); ++iter)
	{
		if ((*iter)->m_bAlive)
		{
			(*iter)->Draw(renderer);
		}
	}
}

void ParticleEmitter2::Spawn()
{
	// Particle Emitter
	m_fMaxLifeSpan = 2.5f;
	m_fAccelerationScalar = 100.0f;
	m_fColor[0] = 0.0f;
	m_fColor[1] = 0.0f;
	m_fColor[2] = 0.0f;
	m_fX = 720.0f;
	m_fY = 450.0f;

	// Particle

	Particle2* newParticle = new Particle2();
	newParticle->Initialize(*m_pSharedSprite);
	newParticle->m_bAlive = true;
	newParticle->m_fMaxLifeSpan = m_fMaxLifeSpan;
	newParticle->m_position = { m_fX, m_fY };

	const float PI = 3.14159f;
	float angleInRadians = (m_fAngle * PI) / 180.0f;
	float velX = -(1.0f * cosf(angleInRadians) - 1.0f * sinf(angleInRadians));
	float velY = -(1.0f * sinf(angleInRadians) + 1.0f * cosf(angleInRadians));
	newParticle->m_velocity = { velX, velY };

	newParticle->m_acceleration = { 100 * velX,100 * velY};
	newParticle->m_fColor[0] = m_fColor[0];
	newParticle->m_fColor[1] = m_fColor[1];
	newParticle->m_fColor[2] = m_fColor[2];

	m_particles.push_back(newParticle);
	


}

void ParticleEmitter2::DebugDraw()
{
	ImGui::SliderFloat("Spawn Angle", &m_fAngle, 0.0f, 360.0f);
	ImGui::Text("Number of particles: %d", m_particles.size());
	/*ImGui::Text("Particles to Spawn:");
	ImGui::SliderInt("Num Particles", &m_iSpawnBatchSize, 0, 500);*/
}
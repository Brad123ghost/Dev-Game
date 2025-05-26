// COMP710 GP Framework 2024

#include "particle.h"
#include "renderer.h"
#include "sprite.h"
#include "inlinehelper.h"
#include <iostream>

Particle::Particle()
	: m_pParticleSprite(0)
	, m_position(0, 0)
	, m_velocity(0, 0)
	, m_acceleration(0, 0)
	, m_age(0)
	, m_MaxAge(0)
	, m_IsAlive(true)
{
	//m_fGravity = { 0, -9.81 };
}

Particle::~Particle()
{
	delete m_pParticleSprite;
	m_pParticleSprite = 0;
}

bool Particle::Initialize(ParticleInfo info)
{
	m_MaxAge = info.maxLifeTime;
	// Set initial position of sprite
	m_pParticleSprite->SetX(info.x);
	m_pParticleSprite->SetY(info.y);
	/*m_acceleration = { 1 * GetPositiveOrNegative(), 1 * GetPositiveOrNegative() };
	m_velocity.x = GetRandom(1,360) * GetPositiveOrNegative() ;
	m_velocity.y = GetRandom(1,360) * GetPositiveOrNegative();*/
	float angleRange = info.maxAngle - info.minAngle;
	const float PI = 3.14159f;
	float angleInRadians = (angleRange * PI) / 180.0f;
	float velX = -(1.0f * cosf(angleInRadians) - 1.0f * sinf(angleInRadians));
	float velY = -(1.0f * sinf(angleInRadians) + 1.0f * cosf(angleInRadians));
	/*float velX = sinf(angleRange);
	float velY = cosf(angleRange);*/
	m_acceleration = { 5 * velX,  5 * velY};
	m_acceleration *= info.acceleration;
	m_velocity.x = 20 ;
	m_velocity.y = 20 ;
	// Set initial position of particle
	m_position = { info.x, info.y };
	return true;
}

void Particle::Process(float deltaTime)
{
	m_age += deltaTime;
	if (m_age < m_MaxAge)
	{
		m_velocity += m_acceleration * deltaTime;
		m_position += m_velocity * deltaTime;
		//m_acceleration -= m_fGravity;

		m_pParticleSprite->SetX(m_position.x);
		m_pParticleSprite->SetY(m_position.y);

		m_pParticleSprite->Process(deltaTime);
	}
	else
	{
		m_IsAlive = false;
	}
}

void Particle::Draw(Renderer& renderer)
{
	if (m_IsAlive)
	{
		m_pParticleSprite->Draw(renderer);
	}
}

void Particle::SetParticleSprite(Sprite* sprite)
{
	Sprite* tempSprite = new Sprite();
	tempSprite->Initialize(*sprite->GetTexture());
	tempSprite->SetScale(0.01f);
	m_pParticleSprite = tempSprite;
}
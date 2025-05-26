// COMP710 GP Framework 2024

// This include:
#include "particle2.h"
#include "sprite.h"

Particle2::Particle2()
	: m_bAlive(false)
	, m_fCurrentAge(0.0f)
{

}

Particle2::~Particle2()
{

}

bool Particle2::Initialize(Sprite& sprite)
{
	m_pSharedSprite = &sprite;
	return true;
}

void Particle2::Process(float deltaTime)
{
	if (m_bAlive)
	{
		m_fCurrentAge += deltaTime;
		m_velocity += m_acceleration * deltaTime;
		m_position += m_velocity * deltaTime;

		if (m_fCurrentAge > m_fMaxLifeSpan)
		{
			m_bAlive = false;
		}
	}
}

void Particle2::Draw(Renderer& renderer)
{
	if (m_bAlive)
	{
		m_pSharedSprite->SetRedTint(m_fColor[0]);
		m_pSharedSprite->SetGreenTint(m_fColor[1]);
		m_pSharedSprite->SetBlueTint(m_fColor[2]);
		float alpha = 1.0f - (m_fCurrentAge / m_fMaxLifeSpan);
		m_pSharedSprite->SetAlpha(alpha);
		m_pSharedSprite->SetX(m_position.x);
		m_pSharedSprite->SetY(m_position.y);
		m_pSharedSprite->Draw(renderer);
	}
}
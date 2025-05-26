#include "lasercanon.h"

#include "renderer.h"
#include "sprite.h"
#include "inputsystem.h"
#include "box2d.h"

#include <iostream>

LaserCanon::LaserCanon()
	: m_LowerBound(0)
	, m_UpperBound(0)
	, m_fSpeed(0)
{
	
}

LaserCanon::~LaserCanon()
{
	delete m_pSprite;
	m_pSprite = 0;
}

bool LaserCanon::Initialize(Renderer& renderer)
{
	m_pSprite = renderer.CreateSprite("sprites\\ball.png");
	m_pSprite->SetScale(0.05f);
	m_pSprite->SetRedTint(0.0f);
	m_pSprite->SetGreenTint(1.0f);
	m_pSprite->SetBlueTint(0.0f);


	const int SPRITE_WIDTH = m_pSprite->GetWidth();
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();

	m_position.x = (SCREEN_WIDTH / 2.0f);
	// Fixed height for space invaders
	m_position.y = 810;
	m_velocity = { 1,1 };
	
	m_pSprite->SetX(m_position.x);
	m_pSprite->SetY(m_position.y);
	m_LowerBound = 360;
	m_UpperBound = 1090 - m_pSprite->GetWidth() /2.0f;
	return true;
}

void LaserCanon::Process(float deltaTime, InputSystem& inputSystem)
{
	ButtonState leftArrowState = (inputSystem.GetKeyState(SDL_SCANCODE_LEFT));
	ButtonState rightArrowState = (inputSystem.GetKeyState(SDL_SCANCODE_RIGHT));
	if (leftArrowState == BS_NEUTRAL && rightArrowState == BS_NEUTRAL)
	{
		m_velocity.x = 0;
	}
	if (leftArrowState == BS_HELD && rightArrowState == BS_HELD)
	{
		m_velocity.x = 0;
	}
	else if (leftArrowState == BS_PRESSED || leftArrowState == BS_HELD)
	{
		if (m_position.x > m_LowerBound)
		{
			m_velocity.x = -m_fSpeed;
		}
		else
		{
			m_velocity.x = 0;
		}
	}
	else if ((rightArrowState == BS_PRESSED || rightArrowState == BS_HELD))
	{
		if (m_position.x < m_UpperBound)
		{
			m_velocity.x = m_fSpeed;
		}
		else
		{
			m_velocity.x = 0;
		}
	}
	// Only process x axis
	m_position.x += m_velocity.x * deltaTime * m_fSpeed;
	m_pSprite->SetX(m_position.x);
}

void LaserCanon::Draw(Renderer& renderer, Camera* camera)
{
	if (m_bAlive)
	{
		m_pSprite->Draw(renderer, camera);
	}
	Entity::Draw(renderer);
}

void LaserCanon::Rotate(float direction)
{

}

bool LaserCanon::IsAlive() const
{
	return m_bAlive;
}

void LaserCanon::SetDead()
{
	m_bAlive = false;
}

float LaserCanon::GetRadius()
{
	return m_pSprite->GetWidth() / 2.0f;
}

Vector2 LaserCanon::GetFacingDirection()
{
	return m_position;
}

Vector2& LaserCanon::GetPosition()
{
	return m_position;
}

Vector2& LaserCanon::GetVelocity()
{
	return m_velocity;
}

bool LaserCanon::IsCollidingWith(Entity& toCheck)
{
	float distanceBetween = (((*this).GetPosition() - toCheck.GetPosition()).Length()) - (*this).GetRadius() - toCheck.GetRadius();

	if (distanceBetween <= 0)
	{
		return true;
	}
	return false;
}


void LaserCanon::SetSpeed(float speed)
{
	m_fSpeed = speed;
}

void LaserCanon::DrawDebug()
{

}
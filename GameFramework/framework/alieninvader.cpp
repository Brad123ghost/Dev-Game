#include "alieninvader.h"

#include "renderer.h"
#include "sprite.h"
#include "inputsystem.h"

#include <iostream>

AlienInvader::AlienInvader()
{

}

AlienInvader::~AlienInvader()
{
	delete m_pSprite;
	m_pSprite = 0;
}

bool AlienInvader::Initialize(Renderer& renderer)
{
	m_pSprite = renderer.CreateSprite("sprites\\ball.png");
	m_pSprite->SetScale(0.05f);
	m_pSprite->SetRedTint(1.0f);
	m_pSprite->SetGreenTint(0.0f);
	m_pSprite->SetBlueTint(0.0f);
	const int EDGE_LIMIT = m_pSprite->GetWidth();
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();

	m_velocity = { 0,0 };

	//m_pSprite->SetX(m_position.x);
	//m_pSprite->SetY(m_position.y);


	return true;
}

void AlienInvader::Process(float deltaTime, InputSystem& inputSystem)
{
	
	/*const float SPEED = 30.0f;
	if (m_position.y < 0)
	{
		SetDead();
	}
	m_velocity.y = -SPEED / 2.0f;
	m_position += m_velocity * deltaTime * SPEED;

	m_pSprite->SetY(m_position.y);*/
	/*if (m_position.x < 400)
	{
		m_position.x += 30.0f;
	}
	else
	{
		m_position.x -= 30.0f;
	}*/
	m_pSprite->SetX(m_position.x);
	m_pSprite->SetY(m_position.y);
}

void AlienInvader::Draw(Renderer& renderer, Camera* camera)
{
	if (m_bAlive)
	{
		m_pSprite->Draw(renderer, camera);
	}
	Entity::Draw(renderer);
}

void AlienInvader::MoveLeft()
{
	m_position.x -= 30.0f;
}

void AlienInvader::MoveRight()
{
	m_position.x += 30.0f;
}

void AlienInvader::MoveDown()
{
	m_position.y += 30.0f;
}

void AlienInvader::SetSpawnPosition(Vector2 spawnPos)
{
	m_position.x = spawnPos.x;
	m_position.y = spawnPos.y;

	m_pSprite->SetX(m_position.x);
	m_pSprite->SetY(m_position.y);
}

void AlienInvader::Rotate(float direction)
{

}

bool AlienInvader::IsAlive() const
{
	return m_bAlive;
}

void AlienInvader::SetDead()
{
	m_bAlive = false;
}

float AlienInvader::GetRadius()
{
	return m_pSprite->GetWidth() / 2.0f;
}

Vector2 AlienInvader::GetFacingDirection()
{
	return m_position;
}

Vector2& AlienInvader::GetPosition()
{
	return m_position;
}

Vector2& AlienInvader::GetVelocity()
{
	return m_velocity;
}

bool AlienInvader::IsCollidingWith(Entity& toCheck)
{
	float distanceBetween = (((*this).GetPosition() - toCheck.GetPosition()).Length()) - (*this).GetRadius() - toCheck.GetRadius();

	if (distanceBetween <= 0)
	{
		return true;
	}
	return false;
}

void AlienInvader::DrawDebug()
{

}
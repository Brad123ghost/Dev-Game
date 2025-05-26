#include "laserbullet.h"

#include "renderer.h"
#include "sprite.h"
#include "inputsystem.h"
#include "box2d.h"
#include "inlinehelper.h"
#include "mycontactlistener.h"

#include <iostream>

LaserBullet::LaserBullet()
{
	
}

LaserBullet::~LaserBullet()
{
	delete m_pSprite;
	m_pSprite = 0;
}

bool LaserBullet::Initialize(Renderer& renderer, bulletSpawnData data)
{
	const float BULLET_SCALE = 0.05f;
	m_pSprite = renderer.CreateSprite("sprites\\ball.png");
	m_pSprite->SetScale(BULLET_SCALE);
	m_pSprite->SetRedTint(1.0f);
	m_pSprite->SetGreenTint(0.0f);
	m_pSprite->SetBlueTint(1.0f);

	const int SPRITE_WIDTH = m_pSprite->GetWidth();
	const int SPRITE_HEIGHT = m_pSprite->GetHeight();

	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();
	SetBulletSpawnData(data);
	m_velocity = { 0, 1 };
	m_pSprite->SetX(m_position.x);

	m_pSprite->SetY(m_position.y);

	return true;
}

void LaserBullet::Process(float deltaTime, InputSystem& inputSystem)
{

	const float SPEED = 20.0f;
	m_fTimeAlive += deltaTime;
	// Delete bullet based of time
	/*if (m_fTimeAlive > 0.4f)
	{
		SetDead();
	}*/

	// Delete bullet for space invaders
	if (m_position.y < 0.0f)
	{
		SetDead();
	}
	
	// Not the best processing but if it works it works :D
	m_velocity.y = -SPEED ;
	m_position += m_velocity * deltaTime * SPEED;
	//std::cout << m_position.y << std::endl;
}

void LaserBullet::Draw(Renderer& renderer, Camera* camera)
{
	if (m_bAlive)
	{
		m_pSprite->SetY(m_position.y);
		m_pSprite->Draw(renderer, camera);
	}

	Entity::Draw(renderer);
}

void LaserBullet::SetBulletSpawnData(bulletSpawnData data)
{
	m_position = data.spawnPos;
	m_pSprite->SetX(m_position.x);
	m_pSprite->SetY(m_position.y);

	float direction = data.direction;

	float angleInRadians = (direction * b2_pi) / 180.0f;

	/*m_velocity.x = sinf(angleInRadians);
	m_velocity.y = -cosf(angleInRadians);*/
}


void LaserBullet::Rotate(float direction)
{
	m_pSprite->SetAngle(direction);
}

bool LaserBullet::IsAlive() const
{
	return m_bAlive;
}

void LaserBullet::SetDead()
{
	m_bAlive = false;
}

float LaserBullet::GetRadius()
{
	return m_pSprite->GetWidth() / 2.0f;
}

Vector2 LaserBullet::GetFacingDirection()
{
	return m_position;
}

Vector2& LaserBullet::GetPosition()
{
	return m_position;
}

Vector2& LaserBullet::GetVelocity()
{
	return m_velocity;
}

bool LaserBullet::IsCollidingWith(Entity& toCheck)
{
	float distanceBetween = (((*this).GetPosition() - toCheck.GetPosition()).Length()) - (*this).GetRadius() - toCheck.GetRadius();

	if (distanceBetween <= 0)
	{
		return true;
	}
	return false;
}

void LaserBullet::DrawDebug()
{
	
}
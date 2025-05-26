// COMP710 GP Framework 2024

// This include:
#include "entity.h"

// Local includes:
#include "sprite.h"
#include "renderer.h"

Entity::Entity()
	: m_bAlive(true)
	, m_bContacting(false)
	, m_bDrawAABB(false)
{

}

Entity::~Entity()
{

}

void Entity::Draw(Renderer& renderer, Camera* camera)
{
	if(m_bDrawAABB)
	{
		renderer.DrawAABB(-0.5, -0.5, 1, 1);
	}
}

void Entity::DrawAABB(bool b)
{
	m_bDrawAABB = b;
}

bool Entity::Initialize(Renderer& renderer)
{
	return true;
}

void Entity::DrawDebug()
{

}

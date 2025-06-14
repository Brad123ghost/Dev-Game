#include "newentity.h"

NewEntity::NewEntity(size_t id, std::string name, eTag tag, ComponentStorage* storage)
	: m_bAlive(true)
	, m_iId(id)
	, m_strName(name)
	, m_eTag(tag)
	, m_pComponentStorage(storage)
{
}

NewEntity::~NewEntity()
{
	// Destructor logic if needed
}

void NewEntity::SetDead()
{
	m_bAlive = false;
}

bool NewEntity::IsAlive() const
{
	return m_bAlive;
}

eTag NewEntity::GetTag() const
{
	return m_eTag;
}

std::string NewEntity::GetTagString() const
{
	switch (m_eTag)
	{
	case eTag::DEFAULT: return "Default";
	case eTag::PLAYER: return "Player";
	case eTag::ENEMY: return "Enemy";
	case eTag::PROJECTILE: return "Projectile";
	case eTag::ITEM: return "Item";
	default: return "Unknown";
	}
}

size_t NewEntity::GetId() const
{
	return m_iId;
}
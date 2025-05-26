#include "newentity.h"

NewEntity::NewEntity(size_t id, eTag tag, ComponentStorage* storage)
	: m_bAlive(true)
	, m_iId(id)
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

size_t NewEntity::GetId() const
{
	return m_iId;
}
#pragma once
#ifndef __ENTITYMANAGER_H_
#define __ENTITYMANAGER_H_

#include "newentity.h"
#include "componentstorage.h"

#include <vector>
#include <memory>
#include <map>

typedef std::vector<std::shared_ptr<NewEntity>> EntityVec;
typedef std::map<eTag, EntityVec> EntityMap;

class EntityManager
{
public:
	EntityManager();

	void Update();

	std::shared_ptr<NewEntity> CreateEntity(const eTag tag);

	EntityVec& GetEntities();
	EntityVec& GetEntities(const eTag tag);

	void DrawDebug();

private:
	void RemoveDeadEntities(EntityVec& vec);

private:
	EntityVec m_entities;
	EntityMap m_entityMap;
	EntityVec m_entitiesToAdd;
	size_t m_totalEntities;
	ComponentStorage m_componentStorage;
};

#endif // __ENTITYMANAGER_H_
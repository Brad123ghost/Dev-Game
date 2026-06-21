#pragma once
#ifndef __ENTITYMANAGER_H_
#define __ENTITYMANAGER_H_

#include "newentity.h"
#include "componentstorage.h"

#include <vector>
#include <memory>
#include <map>
#include <string>

typedef std::vector<std::shared_ptr<NewEntity>> EntityVec;
typedef std::map<eTag, EntityVec> EntityMap;

class EntityManager
{
public:
	EntityManager();

	void Update();

	std::shared_ptr<NewEntity> CreateEntity(std::string name, const eTag tag);

	EntityVec& GetEntities();
	EntityVec& GetEntities(const eTag tag);

	void DrawDebug();
	//void DrawSpawnerDebug();
	void DrawComboPreview(std::vector<std::string> _loadedTextureKeys, const char* _combo_preview_value, static int* _selectedTextureIndex);

private:
	void RemoveDeadEntities(EntityVec& vec);

private:
	size_t m_selectedEntityId = static_cast<size_t>(-1);
	std::shared_ptr<NewEntity> m_selectedEntity = nullptr;
	EntityVec m_entities;
	EntityMap m_entityMap;
	EntityVec m_entitiesToAdd;
	size_t m_totalEntities;
	ComponentStorage m_componentStorage;
};

#endif // __ENTITYMANAGER_H_
#include "entitymanager.h"
#include "imgui.h"

EntityManager::EntityManager()
	: m_totalEntities(0)
{
}

void EntityManager::Update()
{
	// Add new entities
	for(auto& entity : m_entitiesToAdd)
	{
		m_entities.push_back(entity);
		m_entityMap[entity->GetTag()].push_back(entity);
	}
	// Clear the list after adding
	m_entitiesToAdd.clear();

	// Remove Dead entities
	RemoveDeadEntities(m_entities);

	// Remove dead entities from the map
	for (auto& pair : m_entityMap)
	{
		RemoveDeadEntities(pair.second);
	}
}

std::shared_ptr<NewEntity> EntityManager::CreateEntity(const eTag tag)
{
	std::shared_ptr<NewEntity> newEntity = std::shared_ptr<NewEntity>(new NewEntity(m_totalEntities++, tag, &m_componentStorage));
	m_entitiesToAdd.push_back(newEntity);
	return newEntity;
}

EntityVec& EntityManager::GetEntities()
{
	return m_entities;
}

EntityVec& EntityManager::GetEntities(const eTag tag)
{
	return m_entityMap[tag];
}

void EntityManager::RemoveDeadEntities(EntityVec& vec)
{
	auto it = std::remove_if(vec.begin(), vec.end(), [](const std::shared_ptr<NewEntity>& entity) {
		return !entity->IsAlive();
	});
	vec.erase(it, vec.end());
	/*vec.erase(std::remove_if(vec.begin(), vec.end(),[](const std::shared_ptr<NewEntity>& entity) {
		return !entity->IsAlive(); 
	}),
	vec.end());*/
}

void EntityManager::DrawDebug()
{
	ImGui::Text("Entity Manager Debug");
}
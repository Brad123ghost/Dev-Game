#include "entitymanager.h"
#include "imgui.h"
#include "game.h"
#include "texturemanager.h"
#include "renderer.h"
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

	ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp;
	//ImGuiTableRowFlags rowFlags = ImGuiTableRowFlags_Headers;
	ImGuiTableColumnFlags columnFlags = ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoReorder;
	ImGuiSelectableFlags selectableFlags = ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_AllowItemOverlap;

	ImGui::BeginTable("EntityTable", 3, flags);
	ImGui::TableSetupColumn("ID", columnFlags );
	ImGui::TableSetupColumn("Name", columnFlags );
	ImGui::TableSetupColumn("Tag", columnFlags );
	ImGui::TableHeadersRow();
	static size_t selectedEntityId = static_cast<size_t>(-1);
	for (auto& e : m_entities)
	{
		ImGui::PushID(e->GetId());
		
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		bool isSelected = (selectedEntityId == e->GetId());
		if (ImGui::Selectable("##row", isSelected, selectableFlags))
		{
			selectedEntityId = e->GetId();
			std::cout << selectedEntityId << " selected" << std::endl;
		}
		ImGui::SameLine();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("%zu", e->GetId());
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("Name");
		ImGui::TableSetColumnIndex(2);
		ImGui::Text("%s", e->GetTagString().c_str());

		ImGui::PopID();
	}
	ImGui::EndTable();

	ImGui::BeginChild("EntityDetails", ImVec2(0, 450), true);
	if (selectedEntityId == static_cast<size_t>(-1) || selectedEntityId > m_entities.size())
	{
		ImGui::Text("Select an entity to view its properties.");
		ImGui::EndChild();
		return;
	}
	auto loadedTextures = Game::GetInstance().GetRenderer().GetTextureManager()->GetLoadedTextures();
	if (ImGui::Button("Select Texture"))
	{
		Game::GetInstance().GetRenderer().GetTextureManager()->ToggleSelectTexture();
	}
	Game::GetInstance().GetRenderer().GetTextureManager()->SelectTextureDebugDraw();
	std::map<std::string, Texture*>::iterator it = loadedTextures.begin();
	while (it != loadedTextures.end())
	{
		ImGui::Text("%s", it->first.c_str());
		++it;
		//std::cout << it->first << std::endl;
	}
	Texture* temp = Game::GetInstance().GetRenderer().GetTextureManager()->GetTexture("sprites\\ball.png");
	if (ImGui::Button("Change Texture"))
	{
		m_entities[selectedEntityId]->GetComponent<CSprite>()->GetSprite()->ReplaceTexture(*temp);
	}
	ImGui::Text("Name");
	ImGui::Text("ID: %zu", selectedEntityId);
	ImGui::Text("Tag: %s", m_entities[selectedEntityId]->GetTagString().c_str());
	ImGui::SeparatorText("Properties");


	ImGuiTreeNodeFlags propertyFlags = ImGuiTreeNodeFlags_DefaultOpen;
	ImGuiInputTextFlags transformFlags = ImGuiInputTextFlags_CharsDecimal;
	if (m_entities[selectedEntityId]->GetComponent<CTransform>())
	{
		if (ImGui::CollapsingHeader("Transform", propertyFlags))
		{
			ImGui::BeginTable("Transform", 5);
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Position");
			ImGui::TableNextColumn();
			ImGui::Text("X");
			ImGui::SameLine();
			int x = static_cast<int>(m_entities[selectedEntityId]->GetComponent<CTransform>()->position.x);
			if (ImGui::DragInt("##xpos", &x, 1.f, 0, 0, "%d"))
				m_entities[selectedEntityId]->GetComponent<CTransform>()->position.x = static_cast<float>(x);
			ImGui::TableNextColumn();
			ImGui::Text("Y");
			ImGui::SameLine();
			int y = static_cast<int>(m_entities[selectedEntityId]->GetComponent<CTransform>()->position.y);
			if(ImGui::DragInt("##ypos", &y, 1.f, 0, 0, "%d"))
				m_entities[selectedEntityId]->GetComponent<CTransform>()->position.y = static_cast<float>(y);
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Scale");
			ImGui::TableNextColumn();
			ImGui::Text("X");
			ImGui::SameLine();
			float xscale = m_entities[selectedEntityId]->GetComponent<CTransform>()->scale.x;
			if (ImGui::DragFloat("##xscale", &xscale, 0.1f, 0.0f, 10.0f, "%.2f"))
				m_entities[selectedEntityId]->GetComponent<CTransform>()->scale.x = xscale;
			ImGui::TableNextColumn();
			ImGui::Text("Y");
			ImGui::SameLine();
			float yscale = m_entities[selectedEntityId]->GetComponent<CTransform>()->scale.y;
			if (ImGui::DragFloat("##yscale", &yscale, 0.1f, 0.0f, 10.0f, "%.2f"))
				m_entities[selectedEntityId]->GetComponent<CTransform>()->scale.y = yscale;
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Rotation");
			ImGui::TableNextColumn();
			ImGui::Text("X");
			ImGui::SameLine();
			float rot = m_entities[selectedEntityId]->GetComponent<CTransform>()->rotation;
			if(ImGui::DragFloat("##rotation", &rot, 0.1f, 0.0f, 360.0f, "%.2f", ImGuiSliderFlags_WrapAround))
				m_entities[selectedEntityId]->GetComponent<CTransform>()->rotation = rot;
			ImGui::EndTable();
		}
	}
	if (m_entities[selectedEntityId]->GetComponent<CSprite>())
	{
		auto sprite = m_entities[selectedEntityId]->GetComponent<CSprite>()->GetSprite();
		if (ImGui::CollapsingHeader("Sprite", propertyFlags))
		{
			ImGui::BeginTable("SpriteProperties", 6);		
			ImGui::TableNextRow();
			ImGui::NextColumn();
			ImGui::Text("Texture ID: %d", sprite->GetTextureId());
			ImGui::TableNextRow();
			ImGui::NextColumn();
			ImGui::Text("Texture Path: %s", sprite->GetTexturePath());
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Tint");
			ImGui::TableNextColumn();
			float redTint = sprite->GetRedTint();
			ImGui::Text("R");
			ImGui::SameLine();
			if(ImGui::SliderFloat("##redTint", &redTint, 0.0f, 1.0f, "%.2f"))
				sprite->SetRedTint(redTint);
			ImGui::TableNextColumn();
			float greenTint = sprite->GetGreenTint();
			ImGui::Text("G");
			ImGui::SameLine();
			if (ImGui::SliderFloat("##greenTint", &greenTint, 0.0f, 1.0f, "%.2f"))
				sprite->SetGreenTint(greenTint);
			ImGui::TableNextColumn();
			float blueTint = sprite->GetBlueTint();
			ImGui::Text("B");
			ImGui::SameLine();
			if (ImGui::SliderFloat("##blueTint", &blueTint, 0.0f, 1.0f, "%.2f"))
				sprite->SetBlueTint(blueTint);
			ImGui::TableNextColumn();
			float alpha = sprite->GetAlpha();
			ImGui::Text("A");
			ImGui::SameLine();
			if (ImGui::SliderFloat("##alpha", &alpha, 0.0f, 1.0f, "%.2f"))
				sprite->SetAlpha(alpha);
			ImGui::TableNextColumn();
			if (ImGui::Button("Reset"))
			{
				sprite->SetRedTint(1.0f);
				sprite->SetGreenTint(1.0f);
				sprite->SetBlueTint(1.0f);
				sprite->SetAlpha(1.0f);
			}
			ImGui::EndTable();
			
		}
	}

	ImGui::EndChild();
}
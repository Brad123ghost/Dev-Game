#include "entitymanager.h"
#include "imgui.h"
#include "game.h"
#include "texturemanager.h"
#include "renderer.h"
#include "sprocessinput.h"
#include "texture.h"
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
	/*if (ImGui::Button("Texture Browser"))
	{
		Game::GetInstance().GetRenderer().GetTextureManager()->ToggleSelectTexture();
	}*/
	auto loadedTextureKeys = Game::GetInstance().GetRenderer().GetTextureManager()->GetLoadedTextureKeys();

	Game::GetInstance().GetRenderer().GetTextureManager()->SelectTextureDebugDraw();
	static int selectedTextureIndex = 0;
	const char* combo_preview_value = loadedTextureKeys[selectedTextureIndex].c_str();
	if(ImGui::BeginCombo("Texture List", combo_preview_value))
	{
		for (int n = 0; n < loadedTextureKeys.size(); n++)
		{
			const bool is_selected = (selectedTextureIndex == n);
			if (ImGui::Selectable(loadedTextureKeys[n].c_str(), is_selected))
			{
				selectedTextureIndex = n;
			}
			if (ImGui::IsItemHovered())
			{
				Texture* tex = Game::GetInstance().GetRenderer().GetTextureManager()->GetTexture(loadedTextureKeys[n].c_str());
				if (tex)
				{
					int texWidth = tex->GetWidth();
					int texHeight = tex->GetHeight();
					const float maxPrevWidth = 200.f;
					const float maxPrevHeight = 200.f;

					float scale = 1.0f;
					if (texWidth > 0 && texHeight > 0)
					{
						float scaleX = maxPrevWidth / texWidth;
						float scaleY = maxPrevHeight / texHeight;
						scale = (scaleX < scaleY) ? scaleX : scaleY;
						if (scale > 1.0f) 
							scale = 1.0f;
					}
					ImVec2 prevSize(texWidth * scale, texHeight * scale);

					ImGui::BeginTooltip();
					ImGui::Text("Texture ID: %d", tex->GetTextureId());
					ImGui::Text("Size: %d x %d", tex->GetWidth(), tex->GetHeight());
					ImGui::Image((ImTextureID)(intptr_t)tex->GetTextureId(), prevSize);
					ImGui::EndTooltip();
				}
				else
				{
					ImGui::Text("Texture not found.");
				}
			}
		}
		ImGui::EndCombo();
	}
	Texture* newTexture = Game::GetInstance().GetRenderer().GetTextureManager()->GetTexture(combo_preview_value);
	if (ImGui::Button("Change Texture"))
	{
		m_entities[selectedEntityId]->GetComponent<CSprite>()->GetSprite()->ReplaceTexture(*newTexture);
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
			ImGui::BeginTable("Transform", 6);
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Position");
			ImGui::TableNextColumn();
			ImGui::Text("X");
			ImGui::SameLine();
			Vector2& pos = m_entities[selectedEntityId]->GetComponent<CTransform>()->position;
			ImGui::DragFloat("##xpos", &pos.x, 1.f, 0, 0, "%f");
			ImGui::TableNextColumn();
			ImGui::Text("Y");
			ImGui::SameLine();
			ImGui::DragFloat("##ypos", &pos.y, 1.f, 0, 0, "%f");
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Scale");
			ImGui::TableNextColumn();
			ImGui::Text("X");
			ImGui::SameLine();
			Vector2& scale = m_entities[selectedEntityId]->GetComponent<CTransform>()->scale;
			ImGui::DragFloat("##xscale", &scale.x, 0.01f, 0.1f, 10.0f, "%.2f");
			ImGui::TableNextColumn();
			ImGui::Text("Y");
			ImGui::SameLine();
			ImGui::DragFloat("##yscale", &scale.y, 0.01f, 0.1f, 10.0f, "%.2f");
			ImGui::TableNextColumn();
			if (ImGui::Button("Reset##Scale"))
				scale = Vector2(1.0f, 1.0f);
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Rotation");
			ImGui::TableNextColumn();
			ImGui::Text("X");
			ImGui::SameLine();
			float& rot = m_entities[selectedEntityId]->GetComponent<CTransform>()->rotation;
			ImGui::DragFloat("##rotation", &rot, 0.1f, 0.0f, 360.0f, "%.2f", ImGuiSliderFlags_WrapAround);
			ImGui::TableNextColumn();
			if (ImGui::Button("Reset##Rot"))
				rot = 0.f;

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
	if (m_entities[selectedEntityId]->GetComponent<CInput>())
	{
		if (ImGui::CollapsingHeader("Input", propertyFlags))
		{
			SProcessInput::DrawDebug(*m_entities[selectedEntityId]->GetComponent<CInput>());
		}
	}
	ImGui::EndChild();
}
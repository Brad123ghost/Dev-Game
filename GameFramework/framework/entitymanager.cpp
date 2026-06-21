#include "entitymanager.h"
#include "imgui.h"
#include "game.h"
#include "texturemanager.h"
#include "renderer.h"
#include "sprocessinput.h"
#include "texture.h"
#include "animatedsprite.h"

struct AnimationTransiion2;

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
	// Reset the selectedEntityId if the selected entity has been deleted
	bool found = false;
	for (auto& e : m_entities)
	{
		if (e->GetId() == m_selectedEntityId)
		{
			found = true;
			break;
		}
	}
	if (!found)
		m_selectedEntity = nullptr;

	// Remove dead entities from the map
	for (auto& pair : m_entityMap)
	{
		RemoveDeadEntities(pair.second);
	}
}

std::shared_ptr<NewEntity> EntityManager::CreateEntity(std::string name, const eTag tag)
{
	std::shared_ptr<NewEntity> newEntity = std::shared_ptr<NewEntity>(new NewEntity(m_totalEntities++, name, tag, &m_componentStorage));
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
	auto loadedTextureKeys = Game::GetInstance().GetRenderer().GetTextureManager()->GetLoadedTextureKeys();

	ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp;
	//ImGuiTableRowFlags rowFlags = ImGuiTableRowFlags_Headers;
	ImGuiTableColumnFlags columnFlags = ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoReorder;
	ImGuiSelectableFlags selectableFlags = ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_AllowItemOverlap;

	ImGui::BeginTable("EntityTable", 3, flags);
	ImGui::TableSetupColumn("ID", columnFlags );
	ImGui::TableSetupColumn("Name", columnFlags );
	ImGui::TableSetupColumn("Tag", columnFlags );
	ImGui::TableHeadersRow();
	for (auto& e : m_entities)
	{
		ImGui::PushID(e->GetId());
		
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		bool isSelected = (m_selectedEntityId == e->GetId());
		if (ImGui::Selectable("##row", isSelected, selectableFlags))
		{
			if (isSelected)
				m_selectedEntityId = static_cast<size_t>(-1);
			else
				m_selectedEntityId = e->GetId();
			//std::cout << m_selectedEntityId << " selected" << std::endl;
		}
		ImGui::SameLine();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("%zu", e->GetId());
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("%s", e->GetName().c_str());
		ImGui::TableSetColumnIndex(2);
		ImGui::Text("%s", e->GetTagString().c_str());

		ImGui::PopID();
	}
	ImGui::EndTable();

	ImGui::BeginChild("EntityDetails", ImVec2(0, 450), true);
	auto it = std::find_if(
		m_entities.begin(), m_entities.end(), [this](const std::shared_ptr<NewEntity>& entity) {
			return entity->GetId() == m_selectedEntityId;
		}
	);
	
	if (it == m_entities.end())
	{
		ImGui::Text("Select an entity to view its properties.");
		ImGui::EndChild();
		return;
	}

	m_selectedEntity = *it;

	/*if (ImGui::Button("Texture Browser"))
	{
		Game::GetInstance().GetRenderer().GetTextureManager()->ToggleSelectTexture();
	}*/
	
	// Todo rename this better for the selected entity texture change
	static int selectedTextureIndex = 0;
	const char* combo_preview_value = loadedTextureKeys[selectedTextureIndex].c_str();
	DrawComboPreview(loadedTextureKeys, combo_preview_value, &selectedTextureIndex);
	

	Texture* newTexture = Game::GetInstance().GetRenderer().GetTextureManager()->GetTexture(combo_preview_value);
	if (ImGui::Button("Change Texture"))
	{
		// Get sprite returns a std::shared_ptr, trying to replace a *sprite
		m_selectedEntity->GetComponent<CSprite>()->GetSprite()->ReplaceTexture(*newTexture);
	}

	ImGui::Text("%s", m_selectedEntity->GetName().c_str());
	ImGui::Text("ID: %zu", m_selectedEntity->GetId());
	ImGui::Text("Tag: %s", m_selectedEntity->GetTagString().c_str());

	ImGui::SeparatorText("Properties");
	ImGuiTreeNodeFlags propertyFlags = ImGuiTreeNodeFlags_DefaultOpen;
	ImGuiInputTextFlags transformFlags = ImGuiInputTextFlags_CharsDecimal;
	if (m_selectedEntity->GetComponent<CTransform>())
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
			Vector2& pos = m_selectedEntity->GetComponent<CTransform>()->position;
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
			Vector2& scale = m_selectedEntity->GetComponent<CTransform>()->scale;
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
			float& rot = m_selectedEntity->GetComponent<CTransform>()->rotation;
			ImGui::DragFloat("##rotation", &rot, 0.1f, 0.0f, 360.0f, "%.2f", ImGuiSliderFlags_WrapAround);
			ImGui::TableNextColumn();
			if (ImGui::Button("Reset##Rot"))
				rot = 0.f;

			ImGui::EndTable();
		}
	}
	if (m_selectedEntity->GetComponent<CCollider>())
	{
		auto collider = m_selectedEntity->GetComponent<CCollider>();
		if (ImGui::CollapsingHeader("Collider", propertyFlags))
		{
			ImGui::Text("Active: %s", collider->isActive ? "True" : "False");
			ImGui::SameLine();
			if(ImGui::Button("Reset##Active"))
				collider->isActive = true;
			
			ImGui::Text("Triggered: %s", collider->isTriggered ? "True" : "False");
			ImGui::SameLine();
			if (ImGui::Button("Reset##Triggered"))
				collider->isTriggered = false;

			ImGui::Text("Dropped: %s", collider->isDropped ? "True" : "False");
			ImGui::SameLine();
			if (ImGui::Button("Reset##Dropped"))
				collider->isDropped = true;
			Vector2& colliderSize = collider->size;
			ImGui::DragFloat("##colliderXSize", &colliderSize.x, 1.f, 1.f, 128.f, "%f");
			ImGui::DragFloat("##colliderYSize", &colliderSize.y, 1.f, 1.f, 128.f, "%f");
			//ImGui::Text("X: %f", collider->size.x);
			//ImGui::Text("Y: %f", collider->size.y);

		}
	}
	if (m_selectedEntity->GetComponent<CSprite>())
	{
		auto sprite = m_selectedEntity->GetComponent<CSprite>()->GetSprite();
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
	if (m_selectedEntity->GetComponent<CAnimator>())
	{
		auto animator = m_selectedEntity->GetComponent<CAnimator>();
		if (ImGui::CollapsingHeader("Animator", propertyFlags))
		{
			ImGui::SeparatorText("State");
			ImGui::Text("Current: %s", animator->m_sActiveState.c_str());
			ImGui::Text("Previous: %s", animator->m_sPrevActiveState.c_str());

			ImGui::SeparatorText("Transitions");
			ImGui::BeginTable("Animator Transitions", 2, flags);
			ImGui::TableSetupColumn("fromState", columnFlags);
			ImGui::TableSetupColumn("toState", columnFlags);
			/*ImGui::TableSetupColumn("transitioning");*/
			ImGui::TableHeadersRow();
			for (const auto& from :animator->m_transitions)
			{
				std::vector<AnimationTransition2>::const_iterator it = from.second.begin();
				while (it != from.second.end())
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("%s", from.first.c_str());
					ImGui::TableNextColumn();
					ImGui::Text("%s", it->toState.c_str());
					/*ImGui::TableNextColumn();
					ImGui::Text("%s", it->condition() ? "true" : "false");*/
					++it;
				}
			}
			ImGui::EndTable();

			ImGui::SeparatorText("Animation States");
			ImGui::BeginTable("Animator Debug", 2, flags);
			ImGui::TableSetupColumn("Key", columnFlags);
			ImGui::TableSetupColumn("Sprite Path", columnFlags);
			ImGui::TableHeadersRow();

			for (const auto& anim : animator->m_animations)
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("%s", anim.first.c_str());
				ImGui::TableNextColumn();
				ImGui::Text("%s", anim.second->GetTexturePath());
				int width = anim.second->Sprite::GetWidth();
				int height = anim.second->Sprite::GetHeight();
				const float maxPrevWidth = 400.f;
				const float maxPrevHeight = 400.f;
				float scale = 1.0f;
				if (width > 0 && height > 0)
				{
					float scaleX = maxPrevWidth / width;
					float scaleY = maxPrevHeight / height;
					scale = (scaleX < scaleY) ? scaleX : scaleY;
					if (scale > 1.0f)
						scale = 1.0f;
				}
				ImVec2 prevSize(width * scale, height * scale);
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Image((ImTextureID)(intptr_t)anim.second->GetTextureId(), prevSize);
					ImGui::Text("No. of frames: %d", anim.second->GetTotalFrames());
					ImGui::Text("Frame size: %d by %d", anim.second->GetWidth(), anim.second->GetHeight());
					ImGui::EndTooltip();

				}
			}
			ImGui::EndTable();
		}
	}
	if (m_selectedEntity->GetComponent<CInput>())
	{
		if (ImGui::CollapsingHeader("Input", propertyFlags))
		{
			SProcessInput::DrawDebug(*m_selectedEntity->GetComponent<CInput>());
		}
	}
	ImGui::EndChild();
}

void EntityManager::DrawComboPreview(std::vector<std::string> _loadedTextureKeys, const char* _combo_preview_value, static int* _selectedTextureIndex)
{
	ImGui::Text("Texture List");
	if (ImGui::BeginCombo("##Texture List", _combo_preview_value))
	{
		for (int n = 0; n < _loadedTextureKeys.size(); n++)
		{
			const bool is_selected = (*_selectedTextureIndex == n);
			if (ImGui::Selectable(_loadedTextureKeys[n].c_str(), is_selected))
			{
				*_selectedTextureIndex = n;
			}
			if (ImGui::IsItemHovered())
			{
				Texture* tex = Game::GetInstance().GetRenderer().GetTextureManager()->GetTexture(_loadedTextureKeys[n].c_str());
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
}
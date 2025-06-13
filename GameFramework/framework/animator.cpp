#include "animator.h"
#include "animatedsprite.h"
#include "imgui.h"
#include "renderer.h"
#include <iostream>
#include "logmanager.h"
Animator::Animator()
	: m_sActiveState("")
	, m_vPosition(0, 0)
	, m_bFacingLeft(true)
{
}

Animator::~Animator()
{

	auto iter = m_animations.begin();
	while (iter != m_animations.end())
	{
		delete iter->second;
		iter->second = 0;
		++iter;
	}
	m_animations.clear();

}

void Animator::Process(float deltaTime)
{

	for (const auto& transition : m_transitions[m_sActiveState])
	{
		if (transition.condition())
		{
			SetActiveState(transition.toState.c_str());
			break; // Exit after the first valid transition
		}
	}
	std::string animationName = GetAnimationName(m_sActiveState);
	m_animations[animationName]->SetX(static_cast<int>(m_vPosition.x));
	m_animations[animationName]->SetY(static_cast<int>(m_vPosition.y));
	m_animations[animationName]->Process(deltaTime);

}

void Animator::Draw(Renderer& renderer)
{
	m_animations[GetAnimationName(m_sActiveState)]->Draw(renderer);
}

void Animator::AddAnimation(const char* name, AnimatedSprite* sprite)
{
	m_animations.insert({ name, sprite });
}

void Animator::SetDefaultState(const char* name)
{
	SetActiveState(name);
}

void Animator::SetActiveState(const char* name)
{
	if (name == m_sActiveState || name == m_sLastInputState)
		return;
	std::string animationName = GetAnimationName(name);
	if (m_animations.find(animationName) == m_animations.end())
	{
		m_sLastInputState = name;
		std::string msg = "[Animator]: " + std::string(name) + " state not found in animations.";
		LogManager::GetInstance().Log(msg.c_str());
		return;
	}
	
	/*std::string msg = "[Animator]: Setting active state to " + std::string(name);
	LogManager::GetInstance().Log(msg.c_str());*/
	m_sPrevActiveState = m_sActiveState;
	m_sActiveState = name;
	m_sLastInputState = name;
}

void Animator::AddTransition(const std::string& fromState, const std::string& toState, std::function<bool()> condition)
{
	std::vector<AnimationTransition>& transitions = m_transitions[fromState];
	for (const auto& t : transitions)
	{
		if (t.toState == toState)
		{
			// Already added
			std::cout << "Already added" << std::endl;
			return;
		}
	}
	transitions.push_back({toState, condition});
}

void Animator::DrawDebug()
{
	ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp;
	ImGuiTableColumnFlags columnFlags = ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoReorder;

	ImGui::SeparatorText("Current State");
	ImGui::Text("Current: %s", m_sActiveState.c_str());
	ImGui::Text("Previous: %s", m_sPrevActiveState.c_str());
	ImGui::Text("Facing: " "%s", m_bFacingLeft ? "Left" : "Right");

	ImGui::SeparatorText("Transitions");
	ImGui::BeginTable("Animator Transitions",2, flags);
	ImGui::TableSetupColumn("fromState", columnFlags);
	ImGui::TableSetupColumn("toState", columnFlags);
	/*ImGui::TableSetupColumn("transitioning");*/
	ImGui::TableHeadersRow();
	for (const auto& from : m_transitions)
	{
		std::vector<AnimationTransition>::const_iterator it = from.second.begin();
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
	ImGui::BeginTable("Animator Debug",2, flags);
	ImGui::TableSetupColumn("Key", columnFlags);
	ImGui::TableSetupColumn("Sprite Path", columnFlags);
	ImGui::TableHeadersRow();

	for (const auto& anim : m_animations)
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
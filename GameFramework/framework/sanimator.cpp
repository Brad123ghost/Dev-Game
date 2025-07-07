#include "sanimator.h"
#include "entitymanager.h"
#include "animatedsprite.h"
#include "ctransform.h"
#include "logmanager.h"

SAnimator::SAnimator()
{
}

SAnimator::~SAnimator()
{
	// Cleanup if necessary
}

void SAnimator::ProcessAnimator(float deltaTime, EntityManager* em)
{
	// Process all entities to check condition to change active state
	for (auto& e : em->GetEntities())
	{
		auto animator = e->GetComponent<CAnimator>();
		if (animator)
		{
			for (const auto& transition : animator->m_transitions[animator->m_sActiveState])
			{
				if (transition.condition())
				{
					std::cout << "Current State: " << animator->m_sActiveState << " Transition State: " << transition.toState.c_str() << std::endl;
					SetActiveState(animator, e->GetComponent<CTransform>(), transition.toState.c_str());
					break; // Exit after the first valid transition
				}
			}
		}
	}

	// Process all entities to update the position of the animation based on the transform
	for (auto& e : em->GetEntities())
	{
		auto transform = e->GetComponent<CTransform>();
		auto animator = e->GetComponent<CAnimator>();

		if (animator && transform)
		{
			//std::cout << "Name:" << GetAnimationName(animator) << " X:" << transform->position.x << " Y:" << transform->position.y << std::endl;
			animator->m_animations[GetAnimationName(animator, transform)]->SetX(static_cast<int>(transform->position.x));
			animator->m_animations[GetAnimationName(animator, transform)]->SetY(static_cast<int>(transform->position.y));
		}
	}

	// Process all entities and play the animation which is active
	for (auto& e : em->GetEntities())
	{
		if (e->GetComponent<CAnimator>())
		{
			e->GetComponent<CAnimator>()->m_animations[GetAnimationName(e->GetComponent<CAnimator>(), e->GetComponent<CTransform>())]->Process(deltaTime);
		}
	}
}

void SAnimator::DrawAnimator(Renderer& renderer, EntityManager* em)
{
	// Draw animations for all entities
	for (auto& e : em->GetEntities())
	{
		auto animator = e->GetComponent<CAnimator>();
		if (animator)
		{
			std::string animationName = GetAnimationName(animator, e->GetComponent<CTransform>());
			if (animator->m_animations.find(animationName) != animator->m_animations.end())
				animator->m_animations[animationName]->Draw(renderer);		
		}
	}
}


void SAnimator::SetActiveState(CAnimator* animator, CTransform* transform, const char* state)
{
	if (animator->m_sActiveState == state || animator->m_sLastInputState == state)
		return;
	std::string animationName = GetAnimationName(animator, transform);
	if (animator->m_animations.find(animationName) == animator->m_animations.end())
	{
		animator->m_sLastInputState = state;
		std::string msg = "[SAnimator]: " + std::string(state) + " state not found in animations.";
		LogManager::GetInstance().Log(msg.c_str());
		return;
	}
	animator->m_sPrevActiveState = animator->m_sActiveState;
	animator->m_sActiveState = state;
	animator->m_sLastInputState = state;
}


const std::string SAnimator::GetAnimationName(CAnimator* animator, CTransform* transform)
{
	return animator->m_sActiveState + (transform->facingLeft ? "Left" : "Right");
}
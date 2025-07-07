#pragma once
#ifndef __SANIMATOR_H_
#define __SANIMATOR_H_

#include "vector2.h"
#include <string>
#include <functional>
#include "canimator.h"

class AnimatedSprite;
class Renderer;
class EntityManager;
class CAnimator;
class CTransform;

class SAnimator {
public:
	SAnimator();
	~SAnimator();
	static void ProcessAnimator(float deltaTime, EntityManager* em);
	static void DrawAnimator(Renderer& renderer, EntityManager* em);
	/*void AddAnimation(const char* name, AnimatedSprite* sprite);
	void SetDefaultState(const char* name);*/
	static void SetActiveState(CAnimator* animator, CTransform* transform, const char* state);
	static const std::string& GetActiveState(CAnimator* animator) { return animator->m_sActiveState; }
	static const std::string GetAnimationName(CAnimator* animator, CTransform* transform);
	/*static void SetPosition(CAnimator* animator, float x, float y) { animator->m_vPosition.x = x; animator->m_vPosition.y = y; }
	static void SetPosition(CAnimator* animator,  const Vector2& position) { animator->m_vPosition = position; }*/
	//void AddTransition(const std::string& fromState, const std::string& toState, std::function<bool()> condition);
	/*static void SetFacingLeft(CAnimator* animator) { animator->m_bFacingLeft = true; }
	static void SetFacingRight(CAnimator* animator) { animator->m_bFacingLeft = false; }*/
	//void DrawDebug();

private:
	EntityManager* m_entityManager;
};

#endif // __SANIMATOR_H_
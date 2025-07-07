#pragma once
#include "basecomponent.h"
#include "animator.h"

#include "vector2.h"

#include <map>
#include <vector>
#include <string>
#include <functional>

class AnimatedSprite;

struct AnimationTransition2
{
	std::string toState;
	std::function<bool()> condition;
};

class CAnimator : public BaseComponent
{
public:
	std::map<std::string, std::vector<AnimationTransition2>> m_transitions;
	std::map<std::string, AnimatedSprite*> m_animations;
	std::string m_sActiveState;
	std::string m_sPrevActiveState;
	std::string m_sLastInputState;
	//Vector2 m_vPosition; // Won't need this later as it will pull from the CTransform component
};
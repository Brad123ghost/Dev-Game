#pragma once
#ifndef __ANIMATOR_H_
#define __ANIMATOR_H_

#include "vector2.h"

#include <map>
#include <vector>
#include <string>
#include <functional>

class AnimatedSprite;
class Renderer;

struct AnimationTransition
{
	std::string toState;
	std::function<bool()> condition;
};

class Animator
{
public:
	/*Animator();
	~Animator();
	void Initialize(float frameDuration);
	void Update(float deltaTime);
	void SetTotalFrames(int totalFrames);
	void SetLooping(bool looping);
	void Restart();
	int GetCurrentFrame() const;*/
	Animator();
	~Animator();
	void Process(float deltaTime);
	void Draw(Renderer& renderer);
	void AddAnimation(const char* name, AnimatedSprite* sprite);
	void SetDefaultState(const char* name);
	void SetActiveState(const char* name);
	const std::string& GetActiveState() { return m_sActiveState; }
	const std::string GetAnimationName(const std::string state) { return state + (m_bFacingLeft ? "Left" : "Right"); }
	void SetPosition(float x, float y) { m_vPosition.x = x; m_vPosition.y = y; }
	void SetPosition(const Vector2& position) { m_vPosition = position; }

	void AddTransition(const std::string& fromState, const std::string& toState, std::function<bool()> condition);
	void SetFacingLeft() { m_bFacingLeft = true; }
	void SetFacingRight() { m_bFacingLeft = false; }
	void DrawDebug();

private:
	std::map<std::string, std::vector<AnimationTransition>> m_transitions;
	std::map<std::string, AnimatedSprite*> m_animations;
	std::string m_sActiveState;
	std::string m_sPrevActiveState;
	std::string m_sLastInputState;
	Vector2 m_vPosition;

	// Temp
	bool m_bFacingLeft;
};

#endif // __ANIMATOR_H_
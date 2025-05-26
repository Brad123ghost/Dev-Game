#pragma once
#ifndef __SCENEPAUSESCREEN_H_
#define __SCENEPAUSESCREEN_H_

#include "scene.h"

class Renderer;
class Sprite;
class InputSystem;
class SoundSystem;

class ScenePauseScreen : public Scene
{
public:
	ScenePauseScreen();
	virtual ~ScenePauseScreen();

	virtual bool Initialize(Renderer& renderer, SoundSystem& soundSystem);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);

	
	virtual void DebugDraw();

private:
	ScenePauseScreen(const ScenePauseScreen& scenePauseScreen);
	ScenePauseScreen& operator=(const ScenePauseScreen& scenePauseScreen);

public:
protected:
	Sprite* m_pPauseText;
	Sprite* m_pResumeBtn;
	Sprite* m_pResetBtn;
	Sprite* m_pExitBtn;

	Renderer* m_pRenderer;
private:
};

#endif // __SCENEPAUSESCREEN_H_
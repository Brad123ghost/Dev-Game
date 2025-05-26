#pragma once
// COMP710 GP Framework 2024

#ifndef __SCENEMAINMENU_H_
#define __SCENEMAINMENU_H_

// Local includes:
#include "scene.h"

// Forward declarations:
class Renderer;
class Sprite;
class InputSystem;
class SoundSystem;

// Class declaration:
class SceneMainMenu : public Scene
{
	// Member methods:
public:
	SceneMainMenu();
	virtual ~SceneMainMenu();

	virtual bool Initialize(Renderer& renderer, SoundSystem& soundSystem);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);

	virtual void DebugDraw();

protected:

private:
	SceneMainMenu(const SceneMainMenu& sceneIntro);
	SceneMainMenu& operator=(const SceneMainMenu& sceneIntro);

	// Member data:
public:

protected:

	Sprite* m_pStartBtn;
	Sprite* m_pExitBtn;
private:

};

#endif // __SCENEMAINMENU_H_
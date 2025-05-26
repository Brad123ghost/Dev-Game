#pragma once
#ifndef __SCENETESTLEVEL_H_
#define __SCENETESTLEVEL_H_

#include "scene.h"
#include "entitymanager.h"

class Renderer;
class InputSystem;
class SoundSystem;
class Sprite;
class Camera;

class SceneTestLevel : public Scene
{
public:
	SceneTestLevel();
	virtual ~SceneTestLevel();

	virtual bool Initialize(Renderer& renderer, SoundSystem& soundSystem);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);

	virtual void SceneInfoDraw();
	virtual void DebugDraw();
private:
	SceneTestLevel(const SceneTestLevel& sceneTestLevel);
	SceneTestLevel& operator=(const SceneTestLevel& sceneTestLevel);


protected:
	Sprite* m_pTestSprite;
	Camera* m_pCamera;

private:
	bool m_bShowGrid;
	bool m_bDrawAABB;
	int gridSize;
	int cellSize;

	EntityManager m_pEntityManager;
};

#endif // __SCENETESTLEVEL_H_
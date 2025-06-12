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
class Texture;
class AnimatedSprite;

class SceneTestLevel : public Scene
{
public:
	SceneTestLevel();
	virtual ~SceneTestLevel();

	virtual bool Initialize(Renderer& renderer, SoundSystem& soundSystem);
	void replaceTexture(std::shared_ptr<Sprite> sprite, const char* texturePath);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);

	virtual void SceneInfoDraw();
	virtual void DebugDraw();
	virtual void EntityManagerDebugDraw(bool& open);

private:
	SceneTestLevel(const SceneTestLevel& sceneTestLevel);
	SceneTestLevel& operator=(const SceneTestLevel& sceneTestLevel);


protected:
	Sprite* m_pTestSprite;
	Camera* m_pCamera;
	Renderer* m_pRenderer;
private:
	bool m_bShowGrid;
	bool m_bDrawAABB;
	int gridSize;
	int cellSize;
	EntityManager m_entityManager;

	AnimatedSprite* m_pWalkLeft;
	AnimatedSprite* m_pWalkRight;
};

#endif // __SCENETESTLEVEL_H_
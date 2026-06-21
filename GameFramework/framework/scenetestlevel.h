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
class Animator;
class SAnimator;

class SceneTestLevel : public Scene
{
public:
	SceneTestLevel();
	virtual ~SceneTestLevel();

	virtual bool Initialize(Renderer& renderer, SoundSystem& soundSystem);
	void replaceTexture(std::shared_ptr<Sprite> sprite, const char* texturePath);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);

	void ShootBullet(Vector2 spawnPos, float dir);
	//int GetCoins() const { return coins; };
	void AddCoins(int amount);

	virtual void SceneInfoDraw();
	virtual void DebugDraw();
	virtual void EntityManagerDebugDraw(bool& open);
	virtual void EntitySpawnerDebugDraw(bool& open);
	

private:
	SceneTestLevel(const SceneTestLevel& sceneTestLevel);
	SceneTestLevel& operator=(const SceneTestLevel& sceneTestLevel);


protected:
	Sprite* m_pTestSprite;
	Camera* m_pCamera;
	Renderer* m_pRenderer;
	SoundSystem* m_pSoundSystem;

private:
	bool m_bShowGrid;
	bool m_bDrawAABB;
	int gridSize;
	int cellSize;
	int coins;
	EntityManager m_entityManager;

	AnimatedSprite* m_pWalkLeft;
	AnimatedSprite* m_pWalkRight;
	AnimatedSprite* m_pCoinSpin;
	Animator* m_pAnimator;
	//int coins =0;
	//SAnimator* m_pSAnimator;
};

#endif // __SCENETESTLEVEL_H_
#ifndef __SCENESPACEINVADERS_H_
#define __SCENESPACEINVADERS_H_

// Local includes:
#include "scene.h"

// Library includes:
#include <vector>

// Forward declaration:
class Renderer;
class Entity;
class LaserCanon;
class InputSystem;
class Sprite;
class SoundSystem;
class IniParser;
class Camera;

enum class Level
{
	LEVEL_1 = 1,
	LEVEL_2,
	LEVEL_3
};

class SceneSpaceInvaders :public Scene
{
	// Member methods:
public:
	SceneSpaceInvaders();
	virtual ~SceneSpaceInvaders();

	virtual bool Initialize(Renderer& renderer, SoundSystem& soundSystem);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);

	virtual void SceneInfoDraw();
	virtual void DebugDraw();

	void SpawnBullet();
	void SpawnInvaders();
	void StartGame();
	void LoadLevel(Level level);
	void ResetGame();
	void PlaySound(const char* sound);
protected:

private:
	SceneSpaceInvaders(const SceneSpaceInvaders& sceneSpaceInvader);
	SceneSpaceInvaders& operator=(const SceneSpaceInvaders& sceneSpaceInvader);

	// Member data:
public:

protected:
	IniParser* m_pIniParser;

	Sprite* m_pBackground;
	Sprite* m_pText;
	Renderer* m_pRenderer;

	SoundSystem* m_pSoundSystem;

	LaserCanon* m_pLaserCanon;
	std::vector<Entity*> m_pLaserBullet;
	std::vector<Entity*> m_pAlienInvaders;
	
	float m_fPlayerMovementSpeed;
	float m_fPlayerBulletRechargeTimer;
	float m_fPlayerLastBulletTime;

	float m_invaderMoveTimer;

	int m_ALIEN_PER_ROW;
	int m_ALIEN_PER_COLUMN;

	int m_invaderBlockStart;
	int m_invaderBlockEnd;
	int m_defaultInvaderBlockStart;
	int m_defaultInvaderBlockEnd;

	bool m_MoveLeft;
	bool m_MoveDown;

	//Sprite* m_pTileMap[1000][1000];

	Sprite* tempback;
	Sprite* tempback2;
	Sprite* tempback3;

private:
	int m_iMaxWidth;
	int m_iMaxHeight;
	bool m_bDrawAABB;
	bool m_bShowGrid;

	int m_iLowerBound;
	int m_iUpperBound;

	Level m_iCurrentLevel;

	bool m_bEnableCamera;

	Camera* m_pPlayerCamera;

};

#endif __SCENESPACEINVADERS_H_
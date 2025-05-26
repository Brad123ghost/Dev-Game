// COMP710 GP Framework 2024
#ifndef __SCENEBALLGAME_H_
#define __SCENEBALLGAME_H_

// Local includes:
#include "scene.h"

// Library includes:
#include <vector>

// Forward declaration:
class Renderer;
class Ball;
class InputSystem;
class SoundSystem;

// Class declaration:
class SceneBallGame :public Scene
{
	// Member methods:
public:
	SceneBallGame();
	virtual ~SceneBallGame();

	virtual bool Initialize(Renderer& renderer, SoundSystem& soundSystem);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);

	virtual void DebugDraw();

protected:
	void SpawnGoodBalls(int number);
	void SpawnBadBalls(int number);

	void CheckCollisions();
	bool BallVsBall(Ball* p1, Ball* p2);

private:
	SceneBallGame(const SceneBallGame& sceneballgame);
	SceneBallGame& operator=(const SceneBallGame& sceneballgame);

	// Member data:
public:

protected:
	Renderer* m_pRenderer;

	std::vector<Ball*> m_GoodBalls;
	std::vector<Ball*> m_BadBalls;

	Ball* m_PlayerBall;

private:

};

#endif // __SCENEBALLGAME_H_
// COMP710 GP Framework 2024
#ifndef __SCENEBOUNCINGBALLS_H_
#define __SCENEBONUCINGBALLS_H_

// Local includes:
#include "scene.h"

// Forward declarations:
class Renderer;
class Ball;
class InputSystem;
class SoundSystem;

// Class declaration:
class SceneBouncingBalls :public Scene
{
	// Member methods:
public:
	SceneBouncingBalls();
	virtual ~SceneBouncingBalls();

	virtual bool Initialize(Renderer& renderer, SoundSystem& soundSystem);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);

	virtual void DebugDraw();

protected:

private:
	SceneBouncingBalls(const SceneBouncingBalls& sceneBoucingBalls);
	SceneBouncingBalls& operator=(const SceneBouncingBalls& sceneBouncingBalls);

	// Member data:
public:

protected:
	Ball* m_pBalls[100];

	int m_iShowCount;

private:

};

#endif // __SCENEBOUNCINGBALLS_H_
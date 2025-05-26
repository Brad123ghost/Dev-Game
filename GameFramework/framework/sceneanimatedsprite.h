// COMP710 GP Framework 2024
#ifndef __SCENEANIMATEDSPRITE_H_
#define __SCENEANIMATEDSPRITE_H_

// Local includes:
#include "scene.h"

// Forward declarations:
class Renderer;
class InputSystem;
class SoundSystem;
class AnimatedSprite;

// Class declaration:
class SceneAnimatedSprite :public Scene
{
	// Member methods;
public:
	SceneAnimatedSprite();
	virtual ~SceneAnimatedSprite();

	virtual bool Initialize(Renderer& renderer, SoundSystem& soundSystem);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);

	virtual void DebugDraw();

protected:

private:
	SceneAnimatedSprite(const SceneAnimatedSprite& sceneAnimatedSprite);
	SceneAnimatedSprite& operator=(const SceneAnimatedSprite& sceneAnimatedSprite);

	// Member data:
public:

protected:
	AnimatedSprite* m_pAnimatedSprite[3];

private:

};

#endif // __SCENEANIAMTEDSPRITE_H_
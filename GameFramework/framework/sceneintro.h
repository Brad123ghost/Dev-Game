// COMP710 GP Framework 2024

#ifndef __SCENEINTRO_H_
#define __SCENEINTRO_H_

// Local includes:
#include "scene.h"

// Forward declarations:
class Renderer;
class Sprite;
class InputSystem;
class SoundSystem;

// Class declaration:
class SceneIntro : public Scene
{
	// Member methods:
public:
	SceneIntro();
	virtual ~SceneIntro();

	virtual bool Initialize(Renderer& renderer, SoundSystem& soundSystem);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);

	virtual void DebugDraw();

protected:

private:
	SceneIntro(const SceneIntro& sceneIntro);
	SceneIntro& operator=(const SceneIntro& sceneIntro);

	// Member data:
public:

protected:
	Sprite* m_pLogoSprite;

	float m_fSpriteAlpha;
	float m_fCurrentTime;
private:

};

#endif // __SCENEINTRO_H_
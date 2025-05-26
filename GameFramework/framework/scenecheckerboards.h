// COMP710 GP Framework 2024

#ifndef __SCENECHECKERBOARDS_H_
#define __SCENECHECKERBOARDS_H_

// Local includes:
#include "scene.h"

// Forward declarations:
class Renderer;
class Sprite;
class InputSystem;
class SoundSystem;

// Class declaration:
class SceneCheckerboards : public Scene
{
	// Member methods:
public:
	SceneCheckerboards();
	virtual ~SceneCheckerboards();

	virtual bool Initialize(Renderer& renderer, SoundSystem& soundSystem);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);

	virtual void DebugDraw();

protected:

private:
	SceneCheckerboards(const SceneCheckerboards& sceneCheckerboards);
	SceneCheckerboards& operator=(const SceneCheckerboards& sceneCheckerboards);

	// Member data:
public:

protected:
	Sprite* m_pCorners[4];
	Sprite* m_pCenter;

	float m_angle;
	float m_rotationSpeed;

private:

};

#endif // __SCENECHECKERBOARDS_H_
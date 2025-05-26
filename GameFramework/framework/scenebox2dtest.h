#ifndef __SCENEBOX2DTEST_H_
#define __SCENEBOX2DTEST_H_

// local include
#include "scene.h"

#include <vector>

// Forward declarations
class Renderer;
class InputSystem;
class SoundSystem;
class b2World;
class Crate;
class Sprite;

class SceneBox2DTest : public Scene {
public:
	SceneBox2DTest();
	virtual ~SceneBox2DTest();

	virtual bool Initialize(Renderer& renderer, SoundSystem& soundSystem);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);

	virtual void DebugDraw();
protected:

private:
	SceneBox2DTest(const SceneBox2DTest& sceneBox2DTest);
	SceneBox2DTest& operator=(const SceneBox2DTest& sceneBox2DTest);

public:
	Sprite* m_pGroundSprite;
	b2World* m_pWorld;

	std::vector<Crate*> m_pCrates;
protected:
private:

};

#endif // __SCENEBOX2DTEST_H_
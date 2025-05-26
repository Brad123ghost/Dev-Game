// COMP710 GP Framework 2024
#ifndef __SCENEPARTICLE_H_
#define __SCENEPARTICLE_H_

// Local includes:
#include "scene.h"

// Library includes:
#include <vector>

// Forward declaration:
class Renderer;
class ParticleEmitter2;
class InputSystem;
class SoundSystem;

// Class declaration:
class SceneParticle :public Scene
{
	// Member methods:
public:
	SceneParticle();
	virtual ~SceneParticle();

	virtual bool Initialize(Renderer& renderer, SoundSystem& soundSystem);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);

	virtual void DebugDraw();

protected:

private:
	SceneParticle(const SceneParticle& sceneballgame);
	SceneParticle& operator=(const SceneParticle& sceneballgame);

	// Member data:
public:
	ParticleEmitter2* m_pParticleEmitter;
protected:

private:

};

#endif // __SCENEPARTICLE_H_
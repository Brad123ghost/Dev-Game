// COMP 710 GP Framework 2024
#ifndef __SCENE_H_
#define __SCENE_H_

// Forward declarations:
class Renderer;
class InputSystem;
class SoundSystem;

// Class declaration:
class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual bool Initialize(Renderer& renderer, SoundSystem& soundSystem) = 0;
	virtual void Process(float deltaTime, InputSystem& inputSystem) = 0;
	virtual void Draw(Renderer& renderer) = 0;

	virtual void SceneInfoDraw();
	virtual void DebugDraw() = 0;

protected:

private:
	Scene(const Scene& scene);
	Scene& operator=(const Scene& scene);

	// Member data:
public:

protected:

private:

};

#endif // __SCENE_H_
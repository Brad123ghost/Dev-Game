// COMP710 GP Framework 2024

#ifndef __SCENEDDLL_H_
#define __SCENEDDLL_H_

// Local includes:
#include "scene.h"
#include "vector2.h"

// Library includes:
#include <vector>

// Forward declarations:
class Renderer;
class Sprite;
class InputSystem;
class SoundSystem;
class Texture;

// Class declaration:
class SceneDDLL : public Scene
{
	// Member methods:
public:
	SceneDDLL();
	virtual ~SceneDDLL();

	virtual bool Initialize(Renderer& renderer, SoundSystem& soundSystem);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);

	void CreateWall(Texture& sharedWallSprite, Vector2 pos);
	void CreateBreakableBrick(Texture& sharedBrickSprite);

	virtual void DebugDraw();

protected:

private:
	SceneDDLL(const SceneDDLL& sceneDDLL);
	SceneDDLL& operator=(const SceneDDLL& sceneDDLL);

	// Member data:
public:

protected:
	float m_fTileHalfWidth;
	float m_fTileHalfHeight;

	Sprite* m_pTempSprite;

	Texture* m_pSharedGreyBrickTexture;
	Texture* m_pSharedRedBrickTexture;

	std::vector<Sprite*> m_pTileMap;

	Sprite* m_TileMap;
private:

};

#endif // __SCENEDDLL_H_
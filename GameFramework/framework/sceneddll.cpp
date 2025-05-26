// COMP710 GP Framework 2024
// This include:
#include "sceneddll.h"

// Local includes:
#include "renderer.h"
#include "sprite.h"
#include "imgui.h"
#include "texture.h"
#include "vector2.h"

// Library includes:
#include <cassert>

SceneDDLL::SceneDDLL()
	: m_fTileHalfWidth(0)
	, m_fTileHalfHeight(0)
	, m_pTempSprite(0)
	, m_pSharedGreyBrickTexture(0)
	, m_pSharedRedBrickTexture(0)
	, m_pTileMap(0)
	, m_TileMap(0)
{

}

SceneDDLL::~SceneDDLL()
{
	//delete m_pTempSprite;
	delete m_pSharedGreyBrickTexture;
	m_pSharedGreyBrickTexture = 0;
	delete m_pSharedRedBrickTexture;
	m_pSharedRedBrickTexture = 0;

	for (std::vector<Sprite*>::iterator iter = m_pTileMap.begin(); iter < m_pTileMap.end(); ++iter)
	{
		delete* iter;
	}
	m_pTileMap.clear();
}

bool SceneDDLL::Initialize(Renderer& renderer, SoundSystem& soundSystem)
{
	m_pSharedGreyBrickTexture = new Texture();
	m_pSharedGreyBrickTexture->Initialize("sprites\\grey_brick.png");

	m_pSharedRedBrickTexture = new Texture();
	m_pSharedRedBrickTexture->Initialize("sprites\\red_brick.png");
	
	m_fTileHalfWidth = m_pSharedGreyBrickTexture->GetWidth() /2.0f;
	m_fTileHalfHeight = m_pSharedGreyBrickTexture->GetHeight()/2.0f;

	m_TileMap = new Sprite[25];

	// Set Sprites scale
	/*m_pSharedGreyBrick->SetScale(0.5f);
	m_pSharedRedBrick->SetScale(0.5f);*/

	// Since sprite size is the same doesn't matter which one to grab;
	// Get the half width and height of the sprites and screen height and width
	/*const int SPRITE_HALF_WIDTH = m_pSharedGreyBrick->GetWidth() / 2;
	const int SPRITE_HALF_HEIGHT = m_pSharedGreyBrick->GetHeight() / 2;
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();

	
	m_pSharedGreyBrick->SetX(SPRITE_HALF_WIDTH + 2);
	m_pSharedGreyBrick->SetY(SPRITE_HALF_HEIGHT + 2);
	m_pSharedRedBrick->SetX(300);
	m_pSharedRedBrick->SetY(300);*/

	/*m_pLevel.push_back(m_pSharedGreyBrick);
	m_pLevel.push_back(m_pSharedRedBrick);*/


		m_TileMap[0].Initialize(*m_pSharedGreyBrickTexture);
		m_TileMap[0].SetScale(0.5f);
		m_TileMap[0].SetX(200);
		m_TileMap[0].SetY(200 + m_fTileHalfHeight);
		m_TileMap[1].Initialize(*m_pSharedRedBrickTexture);
		m_TileMap[1].SetScale(0.5f);
		m_TileMap[1].SetX(200 + m_fTileHalfWidth);
		m_TileMap[1].SetY(200 + m_fTileHalfHeight);
	

	/*CreateWall(*m_pSharedGreyBrickTexture, {350,200});
	CreateWall(*m_pSharedGreyBrickTexture, {500,200});

	

	CreateBreakableBrick(*m_pSharedRedBrickTexture);*/

	return true;
}

void SceneDDLL::Process(float deltaTime, InputSystem& inputSystem)
{
	for (std::vector<Sprite*>::iterator iter = m_pTileMap.begin(); iter < m_pTileMap.end(); ++iter)
	{
		(*iter)->Process(deltaTime);
	}

	m_TileMap[0].Process(deltaTime);
	m_TileMap[1].Process(deltaTime);
	

}

void SceneDDLL::Draw(Renderer& renderer)
{
	for (std::vector<Sprite*>::iterator iter = m_pTileMap.begin(); iter < m_pTileMap.end(); ++iter)
	{
		(*iter)->Draw(renderer);
	}
	m_TileMap[0].Draw(renderer);
	m_TileMap[1].Draw(renderer);
	

}

void SceneDDLL::CreateWall(Texture& sharedWallSprite, Vector2 pos)
{
	Sprite* temp = new Sprite();
	temp->Initialize(sharedWallSprite);
	temp->SetScale(0.5f);


	temp->SetX(pos.x);
	temp->SetY(pos.y);

	m_pTileMap.push_back(temp);
}

void SceneDDLL::CreateBreakableBrick(Texture& sharedBrickSprite)
{
	Sprite* temp = new Sprite();
	temp->Initialize(sharedBrickSprite);
	temp->SetScale(0.5f);
	temp->SetX(400);
	temp->SetY(400);

	m_pTileMap.push_back(temp);
}

void SceneDDLL::DebugDraw()
{
	
}
// COMP710 GP Framework 2024

// This includes:
#include "scenebox2dtest.h"

// Local includes:
#include "renderer.h"
#include "imgui.h"
#include "box2d.h"
#include "crate.h"
#include "sprite.h"

SceneBox2DTest::SceneBox2DTest()
	: m_pWorld(0)
	, m_pCrates(0)
	, m_pGroundSprite(0)
{

}

SceneBox2DTest::~SceneBox2DTest()
{
	delete m_pGroundSprite;
	delete m_pWorld;

	for (std::vector<Crate*>::iterator iter = m_pCrates.begin(); iter < m_pCrates.end(); ++iter)
	{
		delete* iter;
	}
	m_pCrates.clear();
}

bool SceneBox2DTest::Initialize(Renderer& renderer, SoundSystem& soundSystem)
{
	//const int GROUND_SPRITE_WIDTH = m_pGroundSprite->GetWidth();
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();

	m_pGroundSprite = renderer.CreateSprite("sprites\\ground.png");

	b2Vec2 gravity = { 0, 10.0f };
	m_pWorld = new b2World(gravity);

	b2BodyDef ground;
	ground.position.Set((SCREEN_WIDTH / 2.0f) * 0.01f, 750 * 0.01f);

	b2Body* groundBody = m_pWorld->CreateBody(&ground);

	b2PolygonShape shape;
	shape.SetAsBox(5.12f, 0.32f);

	groundBody->CreateFixture(&shape, 0.0f);
	

	b2Vec2 groundPos = groundBody->GetPosition();
	m_pGroundSprite->SetX(groundPos.x *100);
	m_pGroundSprite->SetY(groundPos.y *100);

	Crate* pNewCrate = new Crate();
	pNewCrate->Initialize(renderer, m_pWorld);
	// Set in pixels, method will convert to m
	pNewCrate->SetPosition(SCREEN_WIDTH / 2.0f, 100.0f);
	m_pCrates.push_back(pNewCrate);

	pNewCrate = new Crate();
	pNewCrate->Initialize(renderer, m_pWorld);
	pNewCrate->SetPosition(SCREEN_WIDTH / 2.0f + 64, 300.0f);
	m_pCrates.push_back(pNewCrate);

	pNewCrate = new Crate();
	pNewCrate->Initialize(renderer, m_pWorld);
	pNewCrate->SetPosition(SCREEN_WIDTH / 2.0f, 500.0f);
	m_pCrates.push_back(pNewCrate);

	return true;
}

void SceneBox2DTest::Process(float deltaTime, InputSystem& InputSystem)
{
	m_pWorld->Step(deltaTime, 6, 2);
}

void SceneBox2DTest::Draw(Renderer& renderer)
{
	m_pGroundSprite->Draw(renderer);


	for (std::vector<Crate*>::iterator iter = m_pCrates.begin(); iter < m_pCrates.end(); ++iter)
	{
		(*iter)->Draw(renderer);
	}
}

void SceneBox2DTest::DebugDraw()
{

}
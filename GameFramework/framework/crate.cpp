#include "crate.h"

#include "renderer.h"
#include "box2d.h"
#include "sprite.h"
#include "inlinehelper.h"
#include <iostream>

Crate::Crate()
	: m_pCrateFixutre(0)
	, m_pCrateBody(0)
	, m_pSprite(0)
{

}

Crate::~Crate()
{
	delete m_pSprite;
}

bool Crate::Initialize(Renderer& renderer, b2World* world)
{
	m_pSprite = renderer.CreateSprite("sprites\\crate.png");

	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();

	b2BodyDef crateBD;
	crateBD.type = b2_dynamicBody;
	

	m_pCrateBody = world->CreateBody(&crateBD);
	
	b2PolygonShape crateBox;
	crateBox.SetAsBox(0.62f, 0.62f);

	b2FixtureDef crateFixture = b2FixtureDef();
	crateFixture.shape = &crateBox;
	crateFixture.density = 1.0f;
	crateFixture.friction = 0.3f;

	m_pCrateFixutre = m_pCrateBody->CreateFixture(&crateFixture);

	return true;
}

void Crate::Draw(Renderer& renderer)
{
	b2Vec2 pos = m_pCrateBody->GetPosition();
	
	float rot = ( m_pCrateBody->GetAngle() * 180) / b2_pi;
	m_pSprite->SetAngle(rot +360);
	std::cout << "Rotation: " << m_pSprite->GetAngle() << std::endl;
	m_pSprite->SetX(pos.x * 100);
	m_pSprite->SetY(pos.y * 100);
	m_pSprite->Draw(renderer);
}

void Crate::SetPosition(float x, float y) const
{
	m_pCrateBody->SetTransform(ScreenPosToWorldPos({ x, y }), 0);
}
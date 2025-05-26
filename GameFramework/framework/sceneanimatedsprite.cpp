// COMP710 GP Framework 2024

// This includes:
#include "sceneanimatedsprite.h"

// Local includes:
#include "renderer.h"
#include "imgui.h"
#include "animatedsprite.h"
#include "texture.h";

// Library includes:
#include <cassert>

SceneAnimatedSprite::SceneAnimatedSprite()
	: m_pAnimatedSprite{ 0 }
{

}

SceneAnimatedSprite::~SceneAnimatedSprite()
{
	for (int i = 0; i < 3; ++i)
	{
		delete m_pAnimatedSprite[i];
		m_pAnimatedSprite[i] = nullptr;
	}
}

bool SceneAnimatedSprite::Initialize(Renderer& renderer, SoundSystem& soundSystem)
{
	// 8 Strip
	m_pAnimatedSprite[0] = renderer.CreateAnimatedSprite("sprites\\anim8strip.png");
	m_pAnimatedSprite[0]->SetupFrames(64, 64);
	m_pAnimatedSprite[0]->SetX(100);
	m_pAnimatedSprite[0]->SetY(100);

	// 8 Strip x2
	m_pAnimatedSprite[1] = renderer.CreateAnimatedSprite("sprites\\anim8stripx2sheet.png");
	m_pAnimatedSprite[1]->SetupFrames(64, 64);
	m_pAnimatedSprite[1]->SetX(200);
	m_pAnimatedSprite[1]->SetY(100);

	// Explosion
	m_pAnimatedSprite[2] = renderer.CreateAnimatedSprite("sprites\\explosion.png");
	m_pAnimatedSprite[2]->SetupFrames(66, 66);
	m_pAnimatedSprite[2]->SetX(300);
	m_pAnimatedSprite[2]->SetY(100);
	m_pAnimatedSprite[2]->SetLooping(true);
	m_pAnimatedSprite[2]->Animate();
	m_pAnimatedSprite[2]->SetFrameDuration(0.1f);

	return true;
}

void SceneAnimatedSprite::Process(float deltaTime, InputSystem& InputSystem)
{
	for (int i = 0; i < 3; ++i)
	{
		m_pAnimatedSprite[i]->Process(deltaTime);
	}
}

void SceneAnimatedSprite::Draw(Renderer& renderer)
{
	for (int i = 0; i < 3; ++i)
	{
		m_pAnimatedSprite[i]->Draw(renderer);
	}
}

void SceneAnimatedSprite::DebugDraw()
{
	ImGui::Text("Scene: Animated Sprite");
	
	static int editAnimation = 0;
	ImGui::SliderInt("Choose Sprite", &editAnimation, 0, 2);

	m_pAnimatedSprite[editAnimation]->DebugDraw();
	

}
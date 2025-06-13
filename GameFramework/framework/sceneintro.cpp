// COMP710 GP Framework 2024
// This include:
#include "sceneintro.h"

// Local includes:
#include "renderer.h"
#include "sprite.h"
#include "imgui.h"
#include "texture.h"
#include "inlinehelper.h"
#include <iostream>

// Library includes:
#include <cassert>

SceneIntro::SceneIntro()
	: m_pLogoSprite(0)
	, m_fSpriteAlpha(0)
	, m_fCurrentTime(0)
{

}

SceneIntro::~SceneIntro()
{
	delete m_pLogoSprite;
	m_pLogoSprite = 0;
}

bool SceneIntro::Initialize(Renderer& renderer, SoundSystem& soundSystem)
{

	renderer.SetClearColor(0, 0, 0);
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();
	
	m_pLogoSprite = renderer.CreateSprite("sprites\\fmodlogo.png");
	m_pLogoSprite->SetX(SCREEN_WIDTH / 2.0f);
	m_pLogoSprite->SetY(SCREEN_HEIGHT / 2.0f);
	m_pLogoSprite->SetAlpha(10.0f);
	return true;
}

void SceneIntro::Process(float deltaTime, InputSystem& inputSystem)
{
	m_fCurrentTime += deltaTime;
	m_fSpriteAlpha = EaseInOutQuad(m_fCurrentTime, 0, 1, 2.2f);
	m_pLogoSprite->SetAlpha(m_fSpriteAlpha);
	
}

void SceneIntro::Draw(Renderer& renderer)
{
	m_pLogoSprite->Draw(renderer, SCREEN);
}


void SceneIntro::DebugDraw()
{
	ImGui::Text("Scene: Splash Screen");
}
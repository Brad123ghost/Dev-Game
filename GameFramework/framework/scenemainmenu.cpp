// COMP710 GP Framework 2024
// This include:
#include "scenemainmenu.h"

// Local includes:
#include "renderer.h"
#include "sprite.h"
#include "imgui.h"
#include "texture.h"
#include "inlinehelper.h"
#include <iostream>
#include "inputsystem.h"
#include "statemanager.h"

// Library includes:
#include <cassert>

SceneMainMenu::SceneMainMenu()
	: m_pStartBtn(0)
	, m_pExitBtn(0)
{

}

SceneMainMenu::~SceneMainMenu()
{
	delete m_pStartBtn;
	m_pStartBtn = 0;

	delete m_pExitBtn;
	m_pExitBtn = 0;
}

bool SceneMainMenu::Initialize(Renderer& renderer, SoundSystem& soundSystem)
{

	//renderer.SetClearColor(0, 0, 0);
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();


	m_pStartBtn = renderer.CreateSprite("sprites\\start-btn.png");
	m_pStartBtn->SetX(SCREEN_WIDTH / 2.0f);
	m_pStartBtn->SetY(SCREEN_HEIGHT / 2.0f -128);

	m_pExitBtn = renderer.CreateSprite("sprites\\exit-btn.png");
	m_pExitBtn->SetX(SCREEN_WIDTH / 2.0f);
	m_pExitBtn->SetY(SCREEN_HEIGHT / 2.0f + 128);

	return true;
}


void SceneMainMenu::Process(float deltaTime, InputSystem& inputSystem)
{
	if (inputSystem.GetMouseButtonState(SDL_BUTTON_LEFT))
	{
		Vector2 mousePos = inputSystem.GetMousePosition();
		//std::cout << "Mouse Position: " << mousePos.x << ", " << mousePos.y << std::endl;
		Vector2 startA = { m_pStartBtn->GetX() - m_pStartBtn->GetWidth() / 2.0f, (float)m_pStartBtn->GetY() - m_pStartBtn->GetHeight() / 2.0f };
		Vector2 startB = { startA.x + m_pStartBtn->GetWidth(), startA.y + m_pStartBtn->GetHeight() };

		//std::cout << "Start Button Position: " << startA.x << ", " << startA.y << std::endl;

		if(mousePos .x > startA.x && mousePos.x < startB.x &&
			mousePos.y > startA.y && mousePos.y < startB.y)
		{
			std::cout << "Start button pressed" << std::endl;
			// Transition to the game scene
			StateManager::GetInstance().SetState(GameState::STATE_SPACE_INVADERS);
		}
		else
		{
			Vector2 exitA = { m_pExitBtn->GetX() - m_pExitBtn->GetWidth() / 2.0f, (float)m_pExitBtn->GetY() - m_pExitBtn->GetHeight() / 2.0f };
			Vector2 exitB = { exitA.x + m_pExitBtn->GetWidth(), exitA.y + m_pExitBtn->GetHeight() };

			if(mousePos.x > exitA.x && mousePos.x < exitB.x &&
				mousePos.y > exitA.y && mousePos.y < exitB.y)
			{
				std::cout << "Exit button pressed" << std::endl;
				StateManager::GetInstance().SetState(GameState::STATE_EXIT);
			}
		}
	}
}

void SceneMainMenu::Draw(Renderer& renderer)
{
	m_pStartBtn->Draw(renderer);
	m_pExitBtn->Draw(renderer);
}


void SceneMainMenu::DebugDraw()
{

}
#include "scenepausescreen.h"

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

ScenePauseScreen::ScenePauseScreen()
	: m_pPauseText(0)
	, m_pResumeBtn(0)
	, m_pResetBtn(0)
	, m_pExitBtn(0)
{

}

ScenePauseScreen::~ScenePauseScreen()
{
	delete m_pPauseText;
	m_pPauseText = 0;

	delete m_pResumeBtn;
	m_pResumeBtn = 0;

	delete m_pResetBtn;
	m_pResetBtn = 0;
	
	delete m_pExitBtn;
	m_pExitBtn = 0;
}

bool ScenePauseScreen::Initialize(Renderer& renderer, SoundSystem& soundSystem)
{
	m_pRenderer = &renderer;
	//renderer.SetClearColor(0, 0, 0);
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();

	//m_pPauseText
	renderer.CreateStaticText("Paused", 100);
	m_pPauseText = renderer.CreateSprite("Paused");
	m_pPauseText->SetX(SCREEN_WIDTH / 2.0f);
	m_pPauseText->SetY(SCREEN_HEIGHT / 2.0f - 256);

	m_pResumeBtn = renderer.CreateSprite("sprites\\resume-btn.png");
	m_pResumeBtn->SetX(SCREEN_WIDTH / 2.0f);
	m_pResumeBtn->SetY(SCREEN_HEIGHT / 2.0f - 96);
	m_pResumeBtn->SetWorldPos(renderer.GetDefaultCamera().ScreenToWorld({ static_cast<float>(m_pResumeBtn->GetX()), static_cast<float>(m_pResumeBtn->GetY()) }));
	
	m_pResetBtn = renderer.CreateSprite("sprites\\reset-btn.png");
	m_pResetBtn->SetX(SCREEN_WIDTH / 2.0f);
	m_pResetBtn->SetY(SCREEN_HEIGHT / 2.0f + 64);
	m_pResetBtn->SetWorldPos(renderer.GetDefaultCamera().ScreenToWorld({ static_cast<float>(m_pResetBtn->GetX()), static_cast<float>(m_pResetBtn->GetY()) }));

	m_pExitBtn = renderer.CreateSprite("sprites\\exit-btn.png");
	m_pExitBtn->SetX(SCREEN_WIDTH / 2.0f);
	m_pExitBtn->SetY(SCREEN_HEIGHT / 2.0f + 224);
	m_pExitBtn->SetWorldPos(renderer.GetDefaultCamera().ScreenToWorld({ static_cast<float>(m_pExitBtn->GetX()), static_cast<float>(m_pExitBtn->GetY()) }));

	return true;
}

Vector2 amousePos, aworldPos, aresumeA, aresumeB, aworldResumeA, aworldResumeB, worldResetA, worldResetB, worldExitA, worldExitB;
void ScenePauseScreen::Process(float deltaTime, InputSystem& inputSystem)
{
	Vector2 dir = { 0, 0 };
	if (inputSystem.GetKeyState(SDL_SCANCODE_W)) dir.y = -1;
	if (inputSystem.GetKeyState(SDL_SCANCODE_S)) dir.y = 1;
	if (inputSystem.GetKeyState(SDL_SCANCODE_A)) dir.x = -1;
	if (inputSystem.GetKeyState(SDL_SCANCODE_D)) dir.x = 1;
	//m_pRenderer->GetCamera()->Move(dir, deltaTime);

	amousePos = inputSystem.GetMousePosition();
	aworldPos = {m_pRenderer->GetDefaultCamera().ScreenToWorld(amousePos)};
	aresumeA = { m_pResumeBtn->GetX() - m_pResumeBtn->GetWidth() / 2.0f, (float)m_pResumeBtn->GetY() - m_pResumeBtn->GetHeight() / 2.0f };
	aresumeB = { aresumeA.x + m_pResumeBtn->GetWidth(), aresumeA.y + m_pResumeBtn->GetHeight() };
	Vector2 tempWorld = m_pResumeBtn->GetWorldPos();
	aworldResumeA = {tempWorld.x - m_pResumeBtn->GetWidth() / 2.0f,  tempWorld.y + m_pResumeBtn->GetHeight() / 2.0f };
	aworldResumeB = { aworldResumeA.x + m_pResumeBtn->GetWidth(), aworldResumeA.y - m_pResumeBtn->GetHeight()};

	if (inputSystem.GetMouseButtonState(SDL_BUTTON_LEFT))
	{
		Vector2 mousePos = inputSystem.GetMousePosition();

		Vector2 resetBtnWorldPos = m_pResetBtn->GetWorldPos();
		worldResetA = { resetBtnWorldPos.x - m_pResetBtn->GetWidth() / 2.0f,  resetBtnWorldPos.y + m_pResetBtn->GetHeight() / 2.0f };
		worldResetB = { worldResetA.x + m_pResetBtn->GetWidth(), worldResetA.y - m_pResetBtn->GetHeight() };

		Vector2 exitBtnWorldPos = m_pExitBtn->GetWorldPos();
		worldExitA = { exitBtnWorldPos.x - m_pExitBtn->GetWidth() / 2.0f,  exitBtnWorldPos.y + m_pExitBtn->GetHeight() / 2.0f };
		worldExitB = { worldExitA.x + m_pExitBtn->GetWidth(), worldExitA.y - m_pExitBtn->GetHeight() };

		if (aworldPos.x > aworldResumeA.x && aworldPos.x < aworldResumeB.x &&
			aworldPos.y > aworldResumeB.y && aworldPos.y < aworldResumeA.y)
		{
			std::cout << "World Resume button pressed" << std::endl;
			StateManager::GetInstance().SetState(GameState::STATE_SPACE_INVADERS);
		}
		else if (aworldPos.x > worldResetA.x && aworldPos.x < worldResetB.x &&
				 aworldPos.y > worldResetB.y && aworldPos.y < worldResetA.y)
		{
			std::cout << "World Reset button pressed" << std::endl;
			StateManager::GetInstance().SetState(GameState::STATE_RESET);
		}
		else if (aworldPos.x > worldExitA.x && aworldPos.x < worldExitB.x &&
			aworldPos.y > worldExitB.y && aworldPos.y < worldExitA.y)
		{
			std::cout << "World Exit button pressed" << std::endl;
			StateManager::GetInstance().SetState(GameState::STATE_EXIT);
		}
	}
}

void ScenePauseScreen::Draw(Renderer& renderer)
{
	m_pPauseText->Draw(renderer);
	m_pResumeBtn->Draw(renderer);
	m_pResetBtn->Draw(renderer);
	m_pExitBtn->Draw(renderer);
}



void ScenePauseScreen::DebugDraw()
{
	if (ImGui::Button("Reset Camera"))
		m_pRenderer->GetDefaultCamera().ResetCamera();
	/*ImGui::SeparatorText("Debug info");
	ImGui::Text("Screen Mouse Position");
	ImGui::Text("x: %f, y: %f", amousePos.x, amousePos.y);
	ImGui::Text("World Mouse Position");
	ImGui::Text("x: %f, y: %f", aworldPos.x, aworldPos.y);
	ImGui::Text("Resume Button Screen A");
	ImGui::Text("x: %f, y: %f", aresumeA.x, aresumeA.y);
	ImGui::Text("Resume Button Screen B");
	ImGui::Text("x: %f, y: %f", aresumeB.x, aresumeB.y);
	ImGui::Text("Resume Button World A");
	ImGui::Text("x: %f, y: %f", aworldResumeA.x, aworldResumeA.y);
	ImGui::Text("Resume Button World B");
	ImGui::Text("x: %f, y: %f", aworldResumeB.x, aworldResumeB.y);*/

	ImGui::SeparatorText("Better Debug Info");
	ImGui::SeparatorText("Camera");
	ImGui::Text("Camera Pos: %.3f, %.3f", m_pRenderer->GetDefaultCamera().GetPosition().x, m_pRenderer->GetDefaultCamera().GetPosition().y);
	ImGui::Text("Camera Zoom: %.3f", m_pRenderer->GetDefaultCamera().GetZoom());
	ImGui::BeginTable("debug_table", 2);
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::SeparatorText("Screen");
	ImGui::Text("Mouse Position");
	ImGui::Text("x: %f, y: %.3f", amousePos.x, amousePos.y);
	ImGui::TableNextColumn();
	ImGui::SeparatorText("World");
	ImGui::Text("Mouse Position");
	ImGui::Text("x: %f, y: %.3f", aworldPos.x, aworldPos.y);
	ImGui::EndTable();

	ImGui::SeparatorText("Resume Button");
	ImGui::BeginTable("resume table", 2);
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("Resume Button A");
	ImGui::Text("x: %f, y: %f", aresumeA.x, aresumeA.y);
	ImGui::Text("Resume Button B");
	ImGui::Text("x: %f, y: %f", aresumeB.x, aresumeB.y);
	ImGui::TableNextColumn();
	ImGui::Text("Resume Button A");
	ImGui::Text("x: %f, y: %f", aworldResumeA.x, aworldResumeA.y);
	ImGui::Text("Resume Button B");
	ImGui::Text("x: %f, y: %f", aworldResumeB.x, aworldResumeB.y);
	ImGui::EndTable();
	
	ImGui::SeparatorText("Reset Button");
	ImGui::BeginTable("reset table", 2);
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::TableNextColumn();
	ImGui::Text("Reset Button A");
	ImGui::Text("x: %f, y: %f", worldResetA.x, worldResetA.y);
	ImGui::Text("Reset Button B");
	ImGui::Text("x: %f, y: %f", worldResetB.x, worldResetB.y);
	ImGui::EndTable();

	ImGui::SeparatorText("Exit Button");
	ImGui::BeginTable("exit table", 2);
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::TableNextColumn();
	ImGui::Text("Exit Button A");
	ImGui::Text("x: %f, y: %f", worldExitA.x, worldExitA.y);
	ImGui::Text("Exit Button B");
	ImGui::Text("x: %f, y: %f", worldExitB.x, worldExitB.y);
	ImGui::EndTable();
}
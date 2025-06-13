// COMP710 GP Framework 2024
// This include:
#include "scenecheckerboards.h"

// Local includes:
#include "renderer.h"
#include "sprite.h"
#include "imgui.h"

// Library includes:
#include <cassert>

SceneCheckerboards::SceneCheckerboards()
	: m_pCorners{ 0 }
	, m_pCenter(0)
	, m_angle(0.0f)
	, m_rotationSpeed(0.0f)
{

}

SceneCheckerboards::~SceneCheckerboards()
{
	for (int k = 0; k < 4; ++k)
	{
		delete m_pCorners[k];
	}

	delete m_pCenter;
}

bool SceneCheckerboards::Initialize(Renderer& renderer, SoundSystem& soundSystem)
{
	m_pCenter = renderer.CreateSprite("sprites\\board8x8.png");
	m_pCorners[0] = renderer.CreateSprite("sprites\\board8x8.png");
	m_pCorners[1] = renderer.CreateSprite("sprites\\board8x8.png");
	m_pCorners[2] = renderer.CreateSprite("sprites\\board8x8.png");
	m_pCorners[3] = renderer.CreateSprite("sprites\\board8x8.png");
	
	const int BOARD_HALF_WIDTH = m_pCenter->GetWidth() / 2;
	const int BOARD_HALF_HEIGHT = m_pCenter->GetHeight() / 2;
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();

	m_pCenter->SetX(SCREEN_WIDTH / 2);
	m_pCenter->SetY(SCREEN_HEIGHT / 2);

	// Top left white:
	m_pCorners[0]->SetX(BOARD_HALF_WIDTH);
	m_pCorners[0]->SetY(BOARD_HALF_HEIGHT);

	// Top right red:
	m_pCorners[1]->SetX(SCREEN_WIDTH - BOARD_HALF_WIDTH);
	m_pCorners[1]->SetY(BOARD_HALF_HEIGHT);
	m_pCorners[1]->SetGreenTint(0.0f);
	m_pCorners[1]->SetBlueTint(0.0f);

	// Bottom right green:
	m_pCorners[2]->SetX(SCREEN_WIDTH - BOARD_HALF_WIDTH);
	m_pCorners[2]->SetY(SCREEN_HEIGHT - BOARD_HALF_HEIGHT);
	m_pCorners[2]->SetRedTint(0.0f);
	m_pCorners[2]->SetBlueTint(0.0f);
	
	// Bottom left blue:
	m_pCorners[3]->SetX(BOARD_HALF_WIDTH);
	m_pCorners[3]->SetY(SCREEN_HEIGHT - BOARD_HALF_HEIGHT);
	m_pCorners[3]->SetRedTint(0.0f);
	m_pCorners[3]->SetGreenTint(0.0f);

	return true;
}

void SceneCheckerboards::Process(float deltaTime, InputSystem& inputSystem)
{
	for (int k = 0; k < 4; ++k)
	{
		m_pCorners[k]->Process(deltaTime);
	}

	m_angle += m_rotationSpeed * deltaTime;

	m_pCenter->SetAngle(m_angle);
	m_pCenter->Process(deltaTime);
}

void SceneCheckerboards::Draw(Renderer& renderer)
{
	for (int k = 0; k < 4; ++k)
	{
		m_pCorners[k]->Draw(renderer);
	}

	m_pCenter->Draw(renderer);
}

void SceneCheckerboards::DebugDraw()
{
	ImGui::Text("Scene: Checkerboards");

	ImGui::InputFloat("Rotation speed", &m_rotationSpeed);

	Vector2 scale = m_pCenter->GetScale();
	ImGui::SliderFloat("Demo scale X", &scale.x, 0.0f, 2.0f, "%.3f");
	ImGui::SliderFloat("Demo scale Y", &scale.y, 0.0f, 2.0f, "%.3f");
	m_pCenter->SetScale(scale);

	int position[2];
	position[0] = m_pCenter->GetX();
	position[1] = m_pCenter->GetY();
	ImGui::InputInt2("Demo position", position);
	m_pCenter->SetX(position[0]);
	m_pCenter->SetY(position[1]);

	float tint[4];
	tint[0] = m_pCenter->GetRedTint();
	tint[1] = m_pCenter->GetGreenTint();
	tint[2] = m_pCenter->GetBlueTint();
	tint[3] = m_pCenter->GetAlpha();
	ImGui::ColorEdit4("Demo tint", tint);
	m_pCenter->SetRedTint(tint[0]);
	m_pCenter->SetGreenTint(tint[1]);
	m_pCenter->SetBlueTint(tint[2]);
	m_pCenter->SetAlpha(tint[3]);
}
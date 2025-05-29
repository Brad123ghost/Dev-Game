// COMP710 GP Framework 2024

// This include:
#include "ball.h"

// Local includes:
#include "renderer.h"
#include "sprite.h"
#include "inlinehelper.h"
#include "imgui.h"
#include "inputsystem.h"

// Library includes:
#include <cassert>
#include <cstdlib>

// Static members:
float Ball::sm_fBoundaryWidth = 0.0f;
float Ball::sm_fBoundaryHeight = 0.0f;

Ball::Ball()
	: m_pSprite(0)
	, m_bAlive(true)
	, m_bIsPlayer(false)
{

}

Ball::~Ball()
{
	delete m_pSprite;
	m_pSprite = 0;
}

bool Ball::Initialize(Renderer& renderer)
{
	m_pSprite = renderer.CreateSprite("sprites\\ball.png");

	RandomizeSize();

	const float MAX_SPEED = 250.0f;
	const int EDGE_LIMIT = m_pSprite->GetWidth();
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();

	sm_fBoundaryWidth = static_cast<float>(SCREEN_WIDTH);
	sm_fBoundaryHeight = static_cast<float>(SCREEN_HEIGHT);

	m_position.x = static_cast<float>(GetRandom(EDGE_LIMIT, SCREEN_WIDTH - EDGE_LIMIT));
	m_position.y = static_cast<float>(GetRandom(EDGE_LIMIT, SCREEN_HEIGHT - EDGE_LIMIT));

	m_velocity.x = GetRandomPercentage() * MAX_SPEED * GetPositiveOrNegative();
	m_velocity.y = GetRandomPercentage() * MAX_SPEED * GetPositiveOrNegative();

	ComputeBounds(SCREEN_WIDTH, SCREEN_HEIGHT);

	return true;
}

void Ball::Process(float deltaTime, Vector2 mousePos)
{
	if (m_bIsPlayer)
	{
		m_position = mousePos;
		m_velocity *= 0;
	}

	if (m_position.x >= (m_boundaryHigh.x))
	{
		m_position.x = m_boundaryHigh.x;
		m_velocity.x *= -1.0f;
	}
	else if (m_position.x <= (m_boundaryLow.x))
	{
		m_position.x = m_boundaryLow.x;
		m_velocity.x *= -1.0f;
	}

	if (m_position.y >= (m_boundaryHigh.y))
	{
		m_position.y = m_boundaryHigh.y;
		m_velocity.y *= -1.0f;
	}
	else if (m_position.y <= (m_boundaryLow.y))
	{
		m_position.y = m_boundaryLow.y;
		m_velocity.y *= -1.0f;
	}

	

	m_position += m_velocity * deltaTime;
	

	m_pSprite->SetX(static_cast<int>(m_position.x));
	m_pSprite->SetY(static_cast<int>(m_position.y));


	m_pSprite->Process(deltaTime);
}

void Ball::Draw(Renderer& renderer)
{
	if (m_bAlive)
	{
		m_pSprite->Draw(renderer);
	}
}

Vector2& Ball::Position()
{
	return m_position;
}

void Ball::RandomizeColor()
{
	m_pSprite->SetRedTint(GetRandomPercentage());
	m_pSprite->SetGreenTint(GetRandomPercentage());
	m_pSprite->SetBlueTint(GetRandomPercentage());
}

void Ball::RandomizeSize()
{
	float scale = GetRandomPercentage();
	scale *= 0.5f;
	m_pSprite->SetScale(scale);
}

void Ball::ComputeBounds(int width, int height)
{
	m_boundaryLow.x = (m_pSprite->GetWidth() / 2.0f);
	m_boundaryLow.y = (m_pSprite->GetHeight() / 2.0f);

	m_boundaryHigh.x = width - (m_pSprite->GetWidth() / 2.0f);
	m_boundaryHigh.y = height - (m_pSprite->GetHeight() / 2.0f);
}

void Ball::DebugDraw()
{
	ImGui::InputFloat2("Position", reinterpret_cast<float*>(&m_position));
	ImGui::InputFloat2("Velocity", reinterpret_cast<float*>(&m_velocity));

	ImGui::Text("Size (%d, %d)", m_pSprite->GetWidth(), m_pSprite->GetHeight());
	ImGui::Text("Lowerbound (%f, %f)", m_boundaryLow.x, m_boundaryLow.y);
	ImGui::Text("Upperbound (%f, %f)", m_boundaryHigh.x, m_boundaryHigh.y);

	Vector2 spriteScale = m_pSprite->GetScale();
	ImGui::InputFloat("Scale X", &spriteScale.x, 0.05f);
	ImGui::InputFloat("Scale Y", &spriteScale.y, 0.05f);
	m_pSprite->SetScale(spriteScale);
	ComputeBounds(static_cast<int>(sm_fBoundaryWidth), static_cast<int>(sm_fBoundaryHeight));

	float color[4];
	color[0] = m_pSprite->GetRedTint();
	color[1] = m_pSprite->GetGreenTint();
	color[2] = m_pSprite->GetBlueTint();
	color[3] = m_pSprite->GetAlpha();
	ImGui::ColorEdit4("Ball color", color);
	m_pSprite->SetRedTint(color[0]);
	m_pSprite->SetGreenTint(color[1]);
	m_pSprite->SetBlueTint(color[2]);
	m_pSprite->SetAlpha(color[3]);
}

void Ball::SetAsPlayer()
{
	m_bIsPlayer = true;

	// Set Player color
	m_pSprite->SetRedTint(0.8f);
	m_pSprite->SetGreenTint(0.8f);
	m_pSprite->SetBlueTint(0.8f);

	// Spawn player in middle
	m_position = { (sm_fBoundaryWidth / 2.0f),(sm_fBoundaryHeight / 2.0f) };
	m_pSprite->SetX(static_cast<int>(m_position.x));
	m_pSprite->SetY(static_cast<int>(m_position.y));

	// Set Player starting scale
	m_pSprite->SetScale(0.05f);
	ComputeBounds(static_cast<int>(sm_fBoundaryWidth), static_cast<int>(sm_fBoundaryHeight));
}

void Ball::SetGood()
{
	m_pSprite->SetRedTint(0);
	m_pSprite->SetGreenTint(1);
	m_pSprite->SetBlueTint(0);

	m_position.y = m_pSprite->GetHeight() / 2.0f;
	m_pSprite->SetY(static_cast<int>(m_position.y));
}

void Ball::SetBad()
{
	m_pSprite->SetRedTint(1);
	m_pSprite->SetGreenTint(0);
	m_pSprite->SetBlueTint(0);

	m_position.y = sm_fBoundaryHeight - m_pSprite->GetHeight() / 2.0f;
	m_pSprite->SetY(static_cast<int>(m_position.y));
}

void Ball::Shrink()
{
	Vector2 currentScale = m_pSprite->GetScale();
	if (currentScale.x > 0.05)
	{
		m_pSprite->SetScale(currentScale.x - 0.01);
	}
}

void Ball::Enlarge()
{
	Vector2 currentScale = m_pSprite->GetScale();
	if (currentScale.x < 2)
	{
		m_pSprite->SetScale(currentScale.x + 0.01);
	}
}

float Ball::GetRadius()
{
	return m_pSprite->GetWidth() / 2.0f;
}

void Ball::Kill()
{
	m_bAlive = false;
}

bool Ball::IsAlive() const
{
	return m_bAlive;
}
// COMP710 GP Framework 2024

// This includes:
#include "sceneballgame.h"

// Local includes:
#include "renderer.h"
#include "ball.h"
#include "imgui.h"
#include "logmanager.h"
#include "inputsystem.h"

// Library includes:
#include <cassert>
#include <string>
#include <iostream>

SceneBallGame::SceneBallGame()
	: m_pRenderer(0)
	, m_GoodBalls(0)
	, m_BadBalls(0)
	, m_PlayerBall(0)
{

}

SceneBallGame::~SceneBallGame()
{
	delete m_PlayerBall;

	for (std::vector<Ball*>::iterator iter = m_GoodBalls.begin(); iter < m_GoodBalls.end(); ++iter)
	{
		delete *iter;
	}
	m_GoodBalls.clear();

	for (std::vector<Ball*>::iterator iter = m_BadBalls.begin(); iter < m_BadBalls.end(); ++iter)
	{
		delete *iter;
	}
	m_BadBalls.clear();
}

bool SceneBallGame::Initialize(Renderer& renderer, SoundSystem& soundSystem)
{
	m_pRenderer = &renderer;
	m_PlayerBall = new Ball();
	m_PlayerBall->Initialize(renderer);
	m_PlayerBall->SetAsPlayer();

	return true;
}

void SceneBallGame::Process(float deltaTime, InputSystem& inputSystem)
{
	Vector2 mousePos = inputSystem.GetMousePosition();
	m_PlayerBall->Process(deltaTime, mousePos);
	CheckCollisions();
	
	if (m_GoodBalls.size() > 0)
	{
		for (std::vector<Ball*>::iterator iter = m_GoodBalls.begin(); iter < m_GoodBalls.end(); ++iter)
		{
			(*iter)->Process(deltaTime, {0,0});
		}
	}

	if (m_BadBalls.size() > 0)
	{
		for (std::vector<Ball*>::iterator iter = m_BadBalls.begin(); iter < m_BadBalls.end(); ++iter)
		{
			(*iter)->Process(deltaTime, { 0,0 });
		}
	}
}

void SceneBallGame::Draw(Renderer& renderer)
{
	m_PlayerBall->Draw(renderer);
	if (m_GoodBalls.size() > 0)
	{
		for (std::vector<Ball*>::iterator iter = m_GoodBalls.begin(); iter < m_GoodBalls.end(); ++iter)
		{
			(*iter)->Draw(renderer);
		}
	}

	if (m_BadBalls.size() > 0)
	{
		for (std::vector<Ball*>::iterator iter = m_BadBalls.begin(); iter < m_BadBalls.end(); ++iter)
		{
			(*iter)->Draw(renderer);
		}
	}
}

void SceneBallGame::DebugDraw()
{
	ImGui::Text("Scene: Ball Game");
	ImGui::Text("Good balls: %d", m_GoodBalls.size());
	ImGui::Text("Bad balls: %d", m_BadBalls.size());
	if (ImGui::Button("Spawn Good Ball"))
	{
		SpawnGoodBalls(1);
	}
	if (ImGui::Button("Spawn 10 Good Balls"))
	{
		SpawnGoodBalls(10);
	}
	if (ImGui::Button("Spawn Bad Ball"))
	{
		SpawnBadBalls(1);
	}
	if (ImGui::Button("Spawn 5 Bad Balls"))
	{
		SpawnBadBalls(5);
	}
}

void SceneBallGame::SpawnGoodBalls(int number)
{
	int current = m_GoodBalls.size();
	

	for (int k = current; k < current+number; ++k)
	{
		m_GoodBalls.push_back(new Ball());
		m_GoodBalls.at(k)->Initialize(*m_pRenderer);
		m_GoodBalls.at(k)->SetGood();
	}

}

void SceneBallGame::SpawnBadBalls(int number)
{
	int current = m_BadBalls.size();

	for (int k = current; k < current + number; ++k)
	{
		m_BadBalls.push_back(new Ball());
		m_BadBalls.at(k)->Initialize(*m_pRenderer);
		m_BadBalls.at(k)->SetBad();
	}
}

void SceneBallGame::CheckCollisions()
{
	if (m_GoodBalls.size() > 0)
	{
		for (auto iter = m_GoodBalls.begin(); iter != m_GoodBalls.end();)
		{
			if (BallVsBall(m_PlayerBall, (*iter)))
			{
				(*iter)->Kill();
				// Freeing the good ball
				delete* iter;
				m_PlayerBall->Enlarge();
				iter = m_GoodBalls.erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}
	if (m_BadBalls.size() > 0)
	{
		for (auto iter = m_BadBalls.begin(); iter != m_BadBalls.end();)
		{
			if (BallVsBall(m_PlayerBall, (*iter)))
			{
				(*iter)->Kill();
				// Freeing the bad ball
				delete* iter;
				m_PlayerBall->Shrink();
				iter = m_BadBalls.erase(iter);
			}
			else
			{
				++iter;
			}
		}

	}
}

bool SceneBallGame::BallVsBall(Ball* p1, Ball* p2)
{
	float distanceBetween = (((*p1).Position() - (*p2).Position()).Length()) - (*p1).GetRadius() - (*p2).GetRadius();

	if (distanceBetween <= 0)
	{
		return true;
	}
	return false;
}
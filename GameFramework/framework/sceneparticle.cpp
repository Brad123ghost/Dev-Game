// COMP710 GP Framework 2024

// This includes:
#include "sceneparticle.h"

// Local includes:
#include "renderer.h"
#include "logmanager.h"
#include "inputsystem.h"
#include "particleemitter2.h"
#include "imgui.h"

// Library includes:
#include <cassert>
#include <string>
#include <iostream>

SceneParticle::SceneParticle()
	: m_pParticleEmitter(0)
{

}

SceneParticle::~SceneParticle()
{
	delete m_pParticleEmitter;
	m_pParticleEmitter = 0;
}

bool SceneParticle::Initialize(Renderer& renderer, SoundSystem& soundSystem)
{
	m_pParticleEmitter = new ParticleEmitter2();
	m_pParticleEmitter->Initialize(renderer);
	return true;
}

void SceneParticle::Process(float deltaTime, InputSystem& inputSystem)
{
	m_pParticleEmitter->Process(deltaTime);
}

void SceneParticle::Draw(Renderer& renderer)
{
	m_pParticleEmitter->Draw(renderer);
}

void SceneParticle::DebugDraw()
{
	m_pParticleEmitter->DebugDraw();
	if (ImGui::Button("Spawn Particles"))
	{
		m_pParticleEmitter->Spawn();
	}
}


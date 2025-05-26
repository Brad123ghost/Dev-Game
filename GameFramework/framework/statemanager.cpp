#include "statemanager.h"
#include "logmanager.h"
#include <iostream>



StateManager* StateManager::sm_pStateInstance = 0;

StateManager& StateManager::GetInstance()
{
	if (sm_pStateInstance == 0)
	{
		sm_pStateInstance = new StateManager();
		LogManager::GetInstance().Log("[Info] StateManager instance created.");
	}
	return (*sm_pStateInstance);
}

void StateManager::DestroyInstance()
{
	delete sm_pStateInstance;
	sm_pStateInstance = 0;
}

StateManager::StateManager()
	: m_eGameState(GameState::STATE_UNINITIALIZED)
{
}

StateManager::~StateManager()
{
}

void StateManager::SetState(GameState state)
{

	m_eGameState = state;
	std::string logMessage = "[State Manager] State changed to: " + GetStateName();
	
	//std::cout << logMessage << std::endl;
	
	LogManager::GetInstance().Log(logMessage.c_str());
}

GameState StateManager::GetState() const
{
	return m_eGameState;
}


#pragma once
#ifndef __STATEMANAGER_H_
#define __STATEMANAGER_H_

#include "gamestate.h"
#include <string>
class StateManager
{

public:
	static StateManager& GetInstance();
	static void DestroyInstance();
	void SetState(GameState state);
	GameState GetState() const;
	std::string GetStateName() const
	{
		switch (m_eGameState)
		{
		case GameState::STATE_UNINITIALIZED:
			return "UNINITIALIZED";
		case GameState::STATE_INTRO:
			return "INTRO";
		case GameState::STATE_MAIN_MENU:
			return "MAIN_MENU";
		case GameState::STATE_PAUSE:
			return "PAUSE";
		case GameState::STATE_SPACE_INVADERS:
			return "SPACE_INVADERS";
		case GameState::STATE_TEST_LEVEL:
			return "TEST_LEVEL";
		case GameState::STATE_RESET:
			return "RESET";
		case GameState::STATE_EXIT:
			return "EXIT";
		default:
			return "UNKNOWN_STATE";
		}
	}

private:
	StateManager();
	~StateManager();
	StateManager(const StateManager& stateManager);
	StateManager& operator=(const StateManager& stateManager);

public:
	

protected:
	static StateManager* sm_pStateInstance;
	GameState m_eGameState;
};


#endif // __STATEMANAGER_H_
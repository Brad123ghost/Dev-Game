#pragma once
#ifndef __GAMESTATE_H_
#define __GAMESTATE_H_
// Need to update statemanager later, currrently the GAMESTATE is the same as the scenes
enum class GameState : int
{
	STATE_UNINITIALIZED = -1,
	STATE_INTRO,
	STATE_MAIN_MENU,
	STATE_PAUSE,
	STATE_SPACE_INVADERS,
	STATE_TEST_LEVEL,
	STATE_RESET,
	STATE_EXIT
};
#endif // __GAMESTATE_H_
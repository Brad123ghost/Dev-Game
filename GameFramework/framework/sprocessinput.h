#pragma once
#ifndef __SPROCESSINPUT_H_
#define __SPROCESSINPUT_H_

#include "entitymanager.h"
#include "inputsystem.h"

class SProcessInput
{
public:
	static void ProcessPlayerInput(float dt, EntityManager& entityManager, InputSystem& inputSystem);
	static void DrawDebug(CInput& cInput);
};

#endif // __SPROCESSINPUT_H_
#pragma once
#ifndef __SPROCESSINPUT_H_
#define __SPROCESSINPUT_H_

#include "entitymanager.h"
#include "inputsystem.h"
#include "camera.h"

class SProcessInput
{
public:
	static void ProcessPlayerInput(float dt, EntityManager& entityManager, InputSystem& inputSystem, const Camera& camera);
	static void DrawDebug(CInput& cInput);
};

#endif // __SPROCESSINPUT_H_
#pragma once
#ifndef __SPROCESSCOLLIDER_H_
#define __SPROCESSCOLLIDER_H_

#include "entitymanager.h"

class SProcessCollider
{
public:
	static void ProcessCollider(float dt, EntityManager& entityManager);
	static void DrawDebug();
};

#endif // __SPROCESSCOLLIDER_H_
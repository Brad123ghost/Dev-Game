#pragma once
#ifndef __CCOLLIDER_H_
#define __COLLLIDER_H_

#include "basecomponent.h"
#include "vector2.h"
#include <functional>

class CCollider : public BaseComponent
{
public:
	Vector2 size;
	bool isActive;
	bool isTriggered;
	bool isDropped;
	
	// ID of object that it collided with.
	//size_t objectID;


	std::function<void()> onEnterCondition;
	std::function<bool()> onLeaveCondition;


	CCollider(Vector2 size = { 1, 1 }, bool active = true, bool dropped = false)
		: size(size)
		, isActive(active)
		, isTriggered(false)
		, isDropped(dropped)
		, onEnterCondition(nullptr)
		, onLeaveCondition(nullptr)
	{
	}
};

#endif // __CCOLLIDER_H_
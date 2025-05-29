#pragma once
#ifndef __CTRANSFORM_H_
#define __CTRANSFORM_H_

#include "vector2.h"
#include "basecomponent.h"

class CTransform : public BaseComponent
{
public:
	Vector2 position;
	Vector2 velocity;
	Vector2 scale;
	float rotation;

	CTransform(Vector2 pos = { 0,0 }, Vector2 vel = { 0,0 }, Vector2 scale = {1.f, 1.f}, float rot = 0.f)
		: position(pos)
		, velocity(vel)
		, scale(scale)
		, rotation(rot)
	{
	};
};
#endif // __CTRANSFORM_H_
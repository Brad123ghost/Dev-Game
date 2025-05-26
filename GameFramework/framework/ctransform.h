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
	float scale;
	float rotation;

	CTransform(const Vector2& pos = { 0,0 }, const Vector2& vel = {0,0}, float scale = 1.f, float rot = 0.f)
		: position(pos)
		, velocity(vel)
		, scale(scale)
		, rotation(rot)
	{
	};
};
#endif // __CTRANSFORM_H_
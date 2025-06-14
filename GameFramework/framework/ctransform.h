#pragma once
#ifndef __CTRANSFORM_H_
#define __CTRANSFORM_H_

#include "vector2.h"
#include "basecomponent.h"
#include "inlinehelper.h"

class CTransform : public BaseComponent
{
public:
	Vector2 position;
	Vector2 worldPosition;
	Vector2 velocity;
	Vector2 dir;
	Vector2 scale;
	float rotation;

	CTransform(Vector2 pos = { 0,0 }, Vector2 vel = { 0,0 }, Vector2 scale = {1.f, 1.f}, float rot = 0.f)
		: position(pos)
		, worldPosition({0,0})
		, velocity(vel)
		, dir({0,0})
		, scale(scale)
		, rotation(rot)
	{

	};
};
#endif // __CTRANSFORM_H_
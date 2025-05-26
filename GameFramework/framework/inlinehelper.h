// COMP710 GP Framework 2024

#ifndef __INLINEHELPERS_H_
#define __INLINEHELPERS_H_
#define b2_VEC_LITERAL(T) T

#include "vector2.h"
#include "box2d.h"
#include "matrix4.h"
#include <cstdlib>
#include <glm.hpp>
#include "camera.h"

inline int GetRandom(int lowerBoundary, int upperBoundary)
{
	int range = (upperBoundary - lowerBoundary + 1);

	return (rand() % range) + lowerBoundary;
}

inline float GetRandomPercentage()
{
	int percent = GetRandom(0, 100);

	return percent / 100.0f;
}

inline float GetPositiveOrNegative()
{
	int coinToss = rand() % 2;

	float result = (coinToss == 0) ? -1.0f : 1.0f;

	return result;
}

inline float EaseInOutQuad(float time, float start, float change, float duration)
{
	time /= (duration / 2);

	if (time < 1.0f)
	{
		return ((change / 2.0f) * (time * time)) + start;
	}

	return -change / 2.0f * (((time - 2.0f) * (--time)) - 1.0f) + start;
}

// Converts the screen position (Vector2) to world position (b2Vec2).
inline b2Vec2 ScreenPosToWorldPos(Vector2 v)
{
	return b2_VEC_LITERAL(b2Vec2) {v.x * 0.01f, v.y * 0.01f};
}

// Converts the world position (b2Vec2) to screen position (Vector2).
inline Vector2 WorldPosToScreenPos(b2Vec2 m)
{
	return b2_VEC_LITERAL(Vector2) { m.x, m.y } * 100.0f;
}

// Converts a float from screen value to world value
inline float ScreenToWorld(float sf)
{
	return sf * 0.01f;
}

// Converts a float from world value to screen value
inline float WorldToScreen(float wf)
{
	return wf * 100.0f;
}

inline Matrix4 Multiply(const Matrix4& a, const Matrix4& b)
{
	Matrix4 result;

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			result.m[i][j] = a.m[i][0] * b.m[0][j] +
			                 a.m[i][1] * b.m[1][j] +
			                 a.m[i][2] * b.m[2][j] +
			                 a.m[i][3] * b.m[3][j];
		}
	}

	return result;
}

inline glm::vec3 ScreenToWorldMouse(const Vector2 mousePos, const glm::mat4 viewMatrix, const glm::mat4 projMatrix, const Vector2 viewportSize)
{
	glm::vec4 ndcPos = { 
		(mousePos.x / viewportSize.x) * 2.0f - 1.0f,
		1.0f - (mousePos.y / viewportSize.y) * 2.0f,
		1.0f,
		1.0f
	};

	glm::vec4 worldPos = glm::inverse(projMatrix * viewMatrix) * ndcPos;
	worldPos /= worldPos.w;

	return glm::vec3(glm::inverse(viewMatrix) * worldPos);
}


inline Vector2 CamScreenToWorld(Vector2 screenPos, const Camera& cam)  {
	Vector2 worldPos;
	worldPos.x = (((screenPos.x - cam.GetViewportSize().x / 2.0f) / cam.GetZoom()) + cam.GetPosition().x);
	worldPos.y = -(((screenPos.y - cam.GetViewportSize().y / 2.0f) / cam.GetZoom()) + cam.GetPosition().y);
	return worldPos;
}
#endif // __INLINEHELPERS_H_
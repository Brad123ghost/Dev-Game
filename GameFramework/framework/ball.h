// COMP710 GP Framework 2024
#ifndef __BALL_H_
#define __BALL_H_

// Local includes:
#include "vector2.h"

// Forward declarations:
class Renderer;
class Sprite;
class InputSystem;

// Class declaration:
class Ball
{
	// Member methods:
public:
	Ball();
	~Ball();

	bool Initialize(Renderer& renderer);
	void Process(float deltaTime, Vector2 mousePos);
	void Draw(Renderer& renderer);

	void RandomizeColor();
	void RandomizeSize();

	Vector2& Position();

	void DebugDraw();

	void SetAsPlayer();
	void SetGood();
	void SetBad();
	void Shrink();
	void Enlarge();
	float GetRadius();
	void Kill();
	bool IsAlive() const;

protected:
	void ComputeBounds(int width, int height);

private:
	Ball(const Ball& ball);
	Ball& operator=(const Ball& ball);

	// Member data:
public:

protected:
	Vector2 m_position;
	Vector2 m_velocity;

	Vector2 m_boundaryLow;
	Vector2 m_boundaryHigh;

	Sprite* m_pSprite;
	bool m_bAlive;
	bool m_bIsPlayer;

	static float sm_fBoundaryWidth;
	static float sm_fBoundaryHeight;

private:

};

#endif // __BALL_H_
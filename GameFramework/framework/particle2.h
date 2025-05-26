// COMP710 GP Framework 2024
#ifndef __PARTICLE2_H_
#define __PARTICLE2_H_

// Local includes:
#include "vector2.h"

// Forward Declarations:
class Renderer;
class Sprite;

// Class declaration:
class Particle2
{
public:
	Particle2();
	~Particle2();

	bool Initialize(Sprite& sprite);
	void Process(float deltaTime);
	void Draw(Renderer& renderer);

protected:

private:
	Particle2(const Particle2& particle);
	Particle2& operator=(const Particle2& particle);


	// Member data:
public:
	Sprite* m_pSharedSprite;
	Vector2 m_position;
	Vector2 m_velocity;
	Vector2 m_acceleration;
	float m_fMaxLifeSpan;
	float m_fCurrentAge;
	float m_fColor[3];
	bool m_bAlive;

protected:
	//Vector2 m_fGravity;

private:

};
#endif // __PARTICLE2_H_
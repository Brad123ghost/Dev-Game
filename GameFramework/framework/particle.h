// COMP710 GP Framework 2024
#ifndef __PARTICLE_H_
#define __PARTICLE_H_

// Library includes:
#include "vector2.h"

// Forward declarations:
class Sprite;
class Renderer;

struct ParticleInfo
{
	float x;
	float y;
	float minAngle;
	float maxAngle;
	float maxLifeTime;
	float acceleration;
};

class Particle
{
public:
	Particle();
	~Particle();

	bool Initialize(ParticleInfo info);
	void Process(float deltaTime);
	void Draw(Renderer& renderer);

	void SetParticleSprite(Sprite* sprite);

protected:

private:

	// Member data:
public:
	Sprite* m_pParticleSprite;

	Vector2 m_position;
	Vector2 m_velocity;
	Vector2 m_acceleration;
	float m_age;
	float m_MaxAge;
	bool m_IsAlive;

protected:
	//Vector2 m_fGravity;
	
private:
};

#endif // __PARTICLE_H_
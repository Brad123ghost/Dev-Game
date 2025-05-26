// COMP710 GP Framework 2024
#ifndef __PARTICLEEMITTER_H_
#define __PARTICLEEMITTER_H_

// Library includes:
#include <vector>

// Forward declarations:
class Renderer;
class Sprite;
class Particle;

// Class declaration:
class ParticleEmitter
{
	// Member methods:
public:
	ParticleEmitter();
	~ParticleEmitter();

	bool Initialize(Renderer& renderer);
	void Process(float deltaTime);
	void Draw(Renderer& renderer);

	void Spawn();
	void DebugDraw();

protected:

private:
	ParticleEmitter(const ParticleEmitter& particleemitter);
	ParticleEmitter& operator=(const ParticleEmitter& particleemitter);

	// Member dataL
public:

protected:
	Sprite* m_pSharedSprite;
	std::vector<Particle*> m_particles;

	float m_fTimeElapsed;

	int m_iSpawnBatchSize;
	float m_fEmitRate;
	float m_fMaxLifeSpan;
	float m_fAccelerationScalar;
	float m_fColour[3];
	float m_fMinAngle;
	float m_fMaxAngle;
	float m_fX;
	float m_fY;

private:

};

#endif // __PARTICLEEMITTER_H_
// COMP710 GP Framework 2024
#ifndef __PARTICLEEMITTER2_H_
#define __PARTICLEEMITTER2_H_

// Library includes:
#include <vector>

// Forward declarations:
class Renderer;
class Sprite;
class Particle2;

// Class declaration:
class ParticleEmitter2
{
	// Member methods:
public:
	ParticleEmitter2();
	~ParticleEmitter2();

	bool Initialize(Renderer& renderer);
	void Process(float deltaTime);
	void Draw(Renderer& renderer);

	void Spawn();
	void DebugDraw();

protected:

private:
	ParticleEmitter2(const ParticleEmitter2& particleemitter);
	ParticleEmitter2& operator=(const ParticleEmitter2& particleemitter);

	// Member dataL
public:

protected:
	Sprite* m_pSharedSprite;
	std::vector<Particle2*> m_particles;

	float m_fTimeElapsed;

	int m_iSpawnBatchSize;
	float m_fEmitRate;
	float m_fMaxLifeSpan;
	float m_fAccelerationScalar;
	float m_fColor[3];
	float m_fAngle;
	float m_fX;
	float m_fY;

private:

};

#endif // __PARTICLEEMITTER_H_
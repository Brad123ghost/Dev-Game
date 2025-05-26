#ifndef __LASERBULLET_H_
#define __LASERBULLET_H_

#include "entity.h"

#include "vector2.h"

class Renderer;
class InputSystem;
class Camera;

typedef struct
{
	Vector2 spawnPos;
	float direction;
} bulletSpawnData;

class LaserBullet : public Entity
{
public:
	LaserBullet();
	virtual ~LaserBullet();
	// bulletSpawnData is a custom struct which can be found in laserbullet.h and parameters include.
	// @param Vector2 - spawn position { x, y }
	// @param float direction
	virtual bool Initialize(Renderer& renderer, bulletSpawnData data);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer, Camera* camera);

	void SetBulletSpawnData(bulletSpawnData data);
	virtual	void Rotate(float direction);

	virtual bool IsAlive() const;
	virtual void SetDead();
	virtual float GetRadius();
	virtual Vector2 GetFacingDirection();

	virtual Vector2& GetPosition();
	virtual Vector2& GetVelocity();

	virtual bool IsCollidingWith(Entity& toCheck);

	virtual void DrawDebug();
protected:
	
private:
	LaserBullet(const LaserBullet& laserBullet);
	LaserBullet& operator=(const LaserBullet& laserBullet);

public:
	
protected:
	float m_fTimeAlive;
	bool m_bBulletAABB;
private:
};

#endif __LASERBULLET_H_
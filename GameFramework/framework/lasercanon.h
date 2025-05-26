#ifndef __LASERCANON_H_
#define __LASERCANON_H_

#include "entity.h"

#include "vector2.h"

class Renderer;
class InputSystem;
class Camera;

class LaserCanon : public Entity
{
public:
	LaserCanon();
	virtual ~LaserCanon();

	virtual bool Initialize(Renderer& renderer);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer, Camera* camera);

	virtual	void Rotate(float direction);

	virtual bool IsAlive() const;
	virtual void SetDead();
	virtual float GetRadius();
	virtual Vector2 GetFacingDirection();

	virtual Vector2& GetPosition();
	virtual Vector2& GetVelocity();

	virtual bool IsCollidingWith(Entity& toCheck);
	void SetSpeed(float speed);

	virtual void DrawDebug();
protected:
	
private:
	LaserCanon(const LaserCanon& laserCanon);
	LaserCanon& operator=(const LaserCanon& laserCanon);

public:

protected:
	float m_fSpeed;

	int m_LowerBound;
	int m_UpperBound;
private:
};

#endif __LASERCANON_H_
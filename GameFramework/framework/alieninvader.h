#ifndef __ALIENINVADER_H_
#define __ALIENINVADER_H_

#include "entity.h"
#include "vector2.h"

class Renderer;
class InputSystem;
class Camera;

class AlienInvader : public Entity
{
public:
	AlienInvader();
	virtual ~AlienInvader();

	virtual bool Initialize(Renderer& renderer);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer, Camera* camera);
	void MoveLeft();
	void MoveRight();
	void MoveDown();
	void SetSpawnPosition(Vector2 spawnPos);
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
	AlienInvader(const AlienInvader& alienInvader);
	AlienInvader& operator=(const AlienInvader& alienInvader);
public:

protected:

private:
};

#endif // __ALIENINVADER_H_
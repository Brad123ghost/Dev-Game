// COMP710 GP Framework 2024
#ifndef __ENTITY_H_
#define __ENTITY_H_

// Local includes:
#include "entity.h"
#include "vector2.h"

// Forward declarations:
class Renderer;
class Sprite;
class InputSystem;
class Camera;

// Class declarations:
class Entity
{
public:
	Entity();
	virtual ~Entity();

	virtual bool Initialize(Renderer& renderer); 
	virtual void Process(float deltaTime, InputSystem& inputSystem) = 0;
	virtual void Draw(Renderer& renderer, Camera* camera = nullptr) = 0;
	void DrawAABB(bool b);

	virtual	void Rotate(float direction) = 0;

	virtual bool IsAlive() const = 0;
	virtual void SetDead() = 0;
	virtual float GetRadius() = 0;
	virtual Vector2 GetFacingDirection() = 0;

	virtual Vector2& GetPosition() = 0;
	virtual Vector2& GetVelocity() = 0;

	virtual bool IsCollidingWith(Entity& toCheck) = 0;

	virtual void DrawDebug() = 0;


protected:

private:
	Entity(const Entity& entity);
	Entity& operator=(const Entity& entity);

	// Member data:
public:
	bool m_bContacting;
protected:
	Sprite* m_pSprite;
	Vector2 m_position;
	Vector2 m_velocity;
	bool m_bAlive;
	bool m_bDrawAABB;
private:

};

#endif // __ENTITY_H_
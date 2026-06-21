#include "sprocesscollider.h"
#include <iostream>

void SProcessCollider::ProcessCollider(float dt, EntityManager& entityManager)
{
	// Double for loop for basic collision detection will change this in the future maybe
	for (auto& a : entityManager.GetEntities())
	{
		CCollider* colliderA = a->GetComponent<CCollider>();
		CTransform* transformA = a->GetComponent<CTransform>();
		if (colliderA && colliderA->isActive)
		{
			for (auto& b : entityManager.GetEntities())
			{
				CCollider* colliderB = b->GetComponent<CCollider>();
				CTransform* transformB = b->GetComponent<CTransform>();
				if (a->GetId() == b->GetId())
					continue;
				else if (colliderB && colliderB->isActive)
				{
					// Horizontal Check
					/*colliderA->isTriggered = transformA->position.x - (colliderA->size.x / 2.0f) > transformB->position.x + (colliderB->size.x / 2.0f) ||
						transformA->position.x + (colliderA->size.x / 2.0f) < transformB->position.x - (colliderB->size.x / 2.0f) &&
						transformA->position.y - (colliderA->size.y / 2.0f) > transformB->position.y + (colliderB->size.y / 2.0f) ||
						transformA->position.y + (colliderA->size.y / 2.0f) < transformB->position.y - (colliderB->size.y / 2.0f);*/

					bool isColliding = 
						transformA->position.x - (colliderA->size.x / 2.0f) < transformB->position.x + (colliderB->size.x / 2.0f) &&
						transformA->position.x + (colliderA->size.x / 2.0f) > transformB->position.x - (colliderB->size.x / 2.0f) &&
						transformA->position.y - (colliderA->size.y / 2.0f) < transformB->position.y + (colliderB->size.y / 2.0f) &&
						transformA->position.y + (colliderA->size.y / 2.0f) > transformB->position.y - (colliderB->size.y / 2.0f);

					if (isColliding)
					{
						// Player hits/pickup item
						if (a->GetTag() == eTag::PLAYER && b->GetTag() == eTag::ITEM)
						{
							if (colliderB->isActive && !colliderB->isTriggered)
							{
								colliderB->isActive = false;
								colliderB->isTriggered = true;
								colliderB->onEnterCondition();
								
								b->SetDead();
							
							}
						}
					}
				}
			}
		}
	}

	//for (auto& e : entityManager.GetEntities())
	//{
	//	CCollider* colliderA = e->GetComponent<CCollider>();
	//	if (colliderA && colliderA->isTriggered && colliderA->isDropped && colliderA->isActive)
	//	{
	//		std::cout << "Triggered" << std::endl;
	//		colliderA->isDropped = false;
	//		//colliderA->isActive = false;
	//		colliderA->onEnterCondition();
	//	}
	//}
}

void SProcessCollider::DrawDebug()
{
	// Draw debug information related to colliders
	// This is a placeholder for the debug drawing logic
	// You can implement drawing collider boundaries or collision states here
}
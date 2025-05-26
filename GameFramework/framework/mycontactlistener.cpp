#include "MyContactListener.h"
#include <iostream>
#include <typeinfo>

MyContactListener::MyContactListener()
{

}

MyContactListener::~MyContactListener()
{

}

void MyContactListener::BeginContact(b2Contact* contact)
{
	uintptr_t temp = contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	uintptr_t temp2 = contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	
	if (temp)
	{
		//std::cout << "Fixture A Pointer found" << std::endl;
		Entity* pB = reinterpret_cast<Entity*>(temp);
		LaserBullet* dp = dynamic_cast<LaserBullet*>(pB);
		if (dp != nullptr)
		{
			dp->StartContact();
		}
	}

	if (temp2)
	{
		//std::cout << "Fixture B Pointer found" << std::endl;
		Entity* pB = reinterpret_cast<Entity*>(temp2);
		LaserBullet* dp = dynamic_cast<LaserBullet*>(pB);
		if (dp != nullptr)
		{
			dp->StartContact();
		}	
	}

	/*void *temp = &contact->GetFixtureA()->GetBody()->GetUserData();
	void *temp2 = &contact->GetFixtureB()->GetBody()->GetUserData();

	Entity *pE = static_cast<Entity*>(temp);
	Entity *pE2 = static_cast<Entity*>(temp2);
	
	LaserBullet* pL = dynamic_cast<LaserBullet*>(pE);*/

	//std::cout << pE->GetEntityType() << std::endl;

	//if (temp)
	//{
	//	std::cout << typeid(temp).name() << std::endl;
	//	std::cout << typeid(pE).name() << std::endl;
	//	std::cout << (*pE).GetEntityType() << std::endl;
	//	//Entity* temp2 = dynamic_cast<Entity*>(temp);
	//	/*Entity* pEntityA = static_cast<Entity*>(contact->GetFixtureA()->GetUserData().pointer);
	//	std::cout << typeid(*pEntityA).name() << std::endl;*/
	//	//std::cout << typeid(*temp2).name() << std::endl;
	//}



	/*std::cout << typeid (contact->GetFixtureA()->GetUserData().pointer).name() << std::endl;
	std::cout << typeid (contact->GetFixtureB()->GetUserData().pointer).name() << std::endl;*/

	/*std::cout << contact->GetFixtureA()->GetUserData().pointer << std::endl;
	std::cout << contact->GetFixtureB()->GetUserData().pointer << std::endl;*/

	/*Entity* pEntityB = reinterpret_cast<Entity*>(contact->GetFixtureB()->GetUserData().pointer);
	

	if (pEntityA->GetEntityType() == ENT_LASER_BULLET && pEntityB->GetEntityType() == ENT_LASER_BULLET)
	{
		pEntityA->StartContact();
		pEntityB->StartContact();
	}*/
}

void MyContactListener::EndContact(b2Contact* contact)
{

	/*Entity* pEntityA = reinterpret_cast<Entity*>(contact->GetFixtureA()->GetUserData().pointer);
	Entity* pEntityB = reinterpret_cast<Entity*>(contact->GetFixtureB()->GetUserData().pointer);


	if (pEntityA->GetEntityType() == ENT_LASER_BULLET || pEntityB->GetEntityType() == ENT_LASER_BULLET)
	{
		pEntityA->EndContact();
		pEntityB->EndContact();
	}*/
	uintptr_t temp = contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	uintptr_t temp2 = contact->GetFixtureB()->GetBody()->GetUserData().pointer;

	if (temp)
	{
		//std::cout << "Fixture A Pointer found" << std::endl;
		Entity* pB = reinterpret_cast<Entity*>(temp);
		LaserBullet* dp = dynamic_cast<LaserBullet*>(pB);
		if (dp != nullptr)
		{
			dp->EndContact();
		}
	}

	if (temp2)
	{
		//std::cout << "Fixture B Pointer found" << std::endl;
		Entity* pB = reinterpret_cast<Entity*>(temp2);
		LaserBullet* dp = dynamic_cast<LaserBullet*>(pB);
		if (dp != nullptr)
		{
			dp->EndContact();
		}
	}
	
}

void MyContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{

}

void MyContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{

}
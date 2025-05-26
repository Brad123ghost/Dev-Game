//#include "collisionquery.h"
//#include "box2d.h"
//#include "laserbullet.h"
//
//void CollisionQuery::BeginContact(b2Contact* contact)
//{
//	void* bodyUserData = &(contact->GetFixtureA()->GetBody()->GetUserData());
//	if (bodyUserData)
//	{
//		static_cast<LaserBullet*>(bodyUserData)->StartContact();
//	}
//}
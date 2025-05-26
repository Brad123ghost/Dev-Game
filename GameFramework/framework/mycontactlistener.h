#pragma once
#ifndef __MYCONTACTLISTENER_H_
#define __MYCONTACTLISTENER_H_

#include "box2d.h"
#include "laserbullet.h"

class MyContactListener: public b2ContactListener
{
public:
	MyContactListener();
	~MyContactListener();

	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);

	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

protected:

private:

public:

protected:

private:
};

#endif // __MYCONTACTLISTENER_H_
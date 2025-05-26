#pragma once
#ifndef _CRATE_H_
#define _CRATE_H_

class Renderer;
class b2World;
class Sprite;
class b2Fixture;
class b2Body;
class b2BodyDef;

class Crate {
public:
	Crate();
	~Crate();

	bool Initialize(Renderer& renderer, b2World* world);
	void Draw(Renderer& renderer);
	void SetPosition(float x, float y) const;
protected:
private:

public:
protected:
	Sprite* m_pSprite;

	b2Body* m_pCrateBody;
	b2Fixture* m_pCrateFixutre;
private:
};

#endif // _CRATE_H_
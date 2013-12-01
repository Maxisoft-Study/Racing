#pragma once
#include "GameObject.h"
class BoxGameObject :
	public GameObject
{
public:
	b2World *getWorld(void) const;
	b2Body *getBody(void) const;
protected:
	BoxGameObject(b2World *world, b2BodyDef *bodydefparam=NULL);
	~BoxGameObject();
	b2BodyDef *bodydef;
	b2Body *body;
};


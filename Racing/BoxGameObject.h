#pragma once
#include "GameObject.h"
class BoxGameObject :
    public GameObject
{
public:
    b2World *getWorld(void) const;
    b2Body *getBody(void) const;
protected:
	BoxGameObject(b2World *world, const float init_pos_x = 0.f, const float init_pos_y = 0.f);
    ~BoxGameObject();
    b2BodyDef *bodydef;
    b2Body *body;
};

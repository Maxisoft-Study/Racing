#pragma once
#include "GameObject.h"
class BoxGameObject :
    public GameObject
{
public:
	
	enum b2xGameObjectCategory{
		CAR_MASK = 1 << 4,
		WHEEL_MASK = 1 << 5,
		CHECKPOINT_MASK = 1 << 6
	};

    b2World *getWorld(void) const;
    b2Body *getBody(void) const;
protected:
	BoxGameObject(b2World *world, const float init_pos_x = 0.f, const float init_pos_y = 0.f);
    ~BoxGameObject();
    b2BodyDef *bodydef;
    b2Body *body;
};

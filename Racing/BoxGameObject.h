#pragma once
#include "GameObject.h"
class BoxGameObject :
	virtual public GameObject
{
public:
	
	enum b2xGameObjectCategory{
		CAR_MASK = 1 << 4,
		WHEEL_MASK = 1 << 5,
		CHECKPOINT_MASK = 1 << 6
	};

    b2World* getWorld(void) const;
    b2Body* getBody(void) const;
protected:
	BoxGameObject(b2World* world);
    ~BoxGameObject();
    b2Body* body;
};

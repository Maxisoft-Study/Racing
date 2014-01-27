#pragma once
#include "GameObject.h"
class BoxGameObject :
	virtual public GameObject
{
public:
	
	enum b2xGameObjectCategory{
		CAR_MASK = 1,
		WHEEL_MASK = 1 << 1,
		CHECKPOINT_MASK = 1 << 2
	};

    b2World* getWorld(void) const;
    b2Body* getBody(void) const;
	//const virtual GameObject::GameObjectTypes getGType(void) const;
protected:
	BoxGameObject(b2World* world);
    ~BoxGameObject();
    b2Body* body;
};

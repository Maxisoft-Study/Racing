#pragma once
#include "GameObject.h"
class BoxGameObject :
	public GameObject
{
public:
	
	enum b2xGameObjectCategory{
		CAR_MASK = 1 << 0,
		WHEEL_MASK = 1 << 1,
		CHECKPOINT_MASK = 1 << 2,
		GROUND_MASK = 1 << 3,
		BARRIERLIMIT_MASK = 1 << 4,
		
	};

    b2World* getWorld(void) const;
    b2Body* getBody(void) const;
	const virtual GameObjectTypes getGType(void) const = 0;
protected:
	explicit BoxGameObject(b2World* world);
    virtual ~BoxGameObject();
    b2Body* body;
};

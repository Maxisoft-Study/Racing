#include "stdafx.h"
#include "BoxGameObject.h"


BoxGameObject::BoxGameObject(b2World *world) : GameObject(), body(NULL)
{
}

BoxGameObject::~BoxGameObject()
{
	if (body)
	{
		getWorld()->DestroyBody(body);
		body = nullptr;
    }
}


b2World* BoxGameObject::getWorld(void) const
{
    return body->GetWorld();
}

b2Body* BoxGameObject::getBody(void) const
{
    return body;
}

/*const GameObject::GameObjectTypes BoxGameObject::getGType(void) const
{
	return GameObject::BoxGameObjectType;
}*/

#include "stdafx.h"
#include "GameObject.h"


GameObject::GameObject() 
{
}

GameObject::~GameObject(void)
{
}

bool GameObject::IsTypeOf(const GameObject &gobj, const GameObjectTypes t)
{
	return (gobj.getGType() & t) != 0;
}

bool GameObject::IsTypeOf(const GameObject* gobj, const GameObjectTypes t)
{
	return (gobj->getGType() & t) != 0;
}

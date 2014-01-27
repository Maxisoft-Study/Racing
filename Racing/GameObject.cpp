#include "stdafx.h"
#include "GameObject.h"


GameObject::GameObject() 
{
}

GameObject::~GameObject(void)
{
}

bool GameObject::IsTypeOf(const GameObject &gobj, const GameObject::GameObjectTypes t)
{
	return (gobj.getGType() & t) != 0;
}

const GameObject::GameObjectTypes GameObject::getGType(void) const
{
	return GameObject::UnsetType;
}

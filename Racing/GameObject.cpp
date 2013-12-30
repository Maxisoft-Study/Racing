#include "stdafx.h"
#include "GameObject.h"


GameObject::GameObject() : gtype(GameObject::UnsetType)
{
}

GameObject::~GameObject(void)
{
}


GameObject::GameObjectTypes GameObject::getGType(void) const
{
	return gtype;
}

void GameObject::setGType(const GameObject::GameObjectTypes typeparam)
{
	gtype = typeparam;
}

bool GameObject::IsThisType(const GameObject &gobj, const GameObject::GameObjectTypes t)
{
	return gobj.getGType() == t;
}

#pragma once
#include "stdafx.h"
class Game;
class GameObject
{
public:
	const virtual GameObjectTypes getGType(void) const = 0;
	static bool IsTypeOf(const GameObject& gobj, const GameObjectTypes t);
protected:
	GameObject();
	~GameObject(void);
	Game* game;
};

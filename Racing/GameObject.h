#pragma once
#include "stdafx.h"
class GameObject;

class GameObject
{
public:
	virtual void update(float delta) = 0;
protected:
	const sf::Vector2u size;
	GameObject(sf::Vector2u vector);
	virtual ~GameObject(void);
};


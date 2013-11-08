#pragma once
#include "stdafx.h"
class GameObject;

class GameObject
{
public:
	const sf::Vector2i getPositionInt(void);
	//virtual void update(float delta) = 0;
protected:
	const sf::Vector2u size;
	sf::Vector2f position;
	GameObject(sf::Vector2u vector);
	virtual ~GameObject(void);
};


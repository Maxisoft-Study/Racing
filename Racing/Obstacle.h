#pragma once
#include "SpriteGameObject.h"
class Obstacle :
	public SpriteGameObject
{
public:
	Obstacle(sf::Vector2u vector, const std::string &file);
	~Obstacle(void);
};


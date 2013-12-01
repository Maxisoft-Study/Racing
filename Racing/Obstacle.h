#pragma once
#include "SpriteGameObject.h"
class Obstacle :
	public SpriteGameObject
{
public:
	Obstacle(const std::string &file);
	~Obstacle(void);
};


#pragma once
#include "BoxGameObject.h"
class Checkpoint :
	public BoxGameObject
{
public:
	Checkpoint(b2World *world, const float init_pos_x, const float init_pos_y, const sf::Vector2u size);
	~Checkpoint(void);
};


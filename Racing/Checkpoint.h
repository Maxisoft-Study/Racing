#pragma once
#include "BoxGameObject.h"
class Checkpoint :
	public BoxGameObject
{
public:
	Checkpoint(b2World *world, const float init_pos_x = 0.f, const float init_pos_y = 0.f);
	~Checkpoint(void);
};


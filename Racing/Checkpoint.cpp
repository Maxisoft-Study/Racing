#include "stdafx.h"
#include "Checkpoint.h"


Checkpoint::Checkpoint(b2World *world, const float init_pos_x, const float init_pos_y) : BoxGameObject(world, init_pos_x, init_pos_y)
{
	setGType(GameObject::CheckpointType);
}


Checkpoint::~Checkpoint()
{
}

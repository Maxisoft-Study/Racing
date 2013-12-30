#include "stdafx.h"
#include "MixedGameObject.h"


MixedGameObject::MixedGameObject(b2World *world, const std::string &file, const float init_pos_x, const float init_pos_y) :
SpriteGameObject(file), BoxGameObject(world, init_pos_x, init_pos_y)
{
}


MixedGameObject::~MixedGameObject()
{
}

void MixedGameObject::update(const float delta)
{
	auto position = getBody()->GetWorldCenter();
	setSpritePosition(Utils::Box2DVectToSfVectPixel(position));
	auto angle = getBody()->GetAngle();
	setSpriteRotation(Utils::RadianToDegree(angle));
}

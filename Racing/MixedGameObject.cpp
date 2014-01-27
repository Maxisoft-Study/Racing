#include "stdafx.h"
#include "MixedGameObject.h"


MixedGameObject::MixedGameObject(b2World *world, const std::string &file) :
SpriteGameObject(file), BoxGameObject(world)
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

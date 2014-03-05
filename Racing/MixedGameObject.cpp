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
	auto spritePosition = Utils::Box2DVectToSfVectPixel(position);
	spritePosition.x = std::round(spritePosition.x);
	spritePosition.y = std::round(spritePosition.y);
	setSpritePosition(spritePosition);
	auto angle = getBody()->GetAngle();
	setSpriteRotation(Utils::RadianToDegree(angle));
}


const GameObjectTypes MixedGameObject::getGType(void) const
{
	return GameObjectTypes::MixedGameObjectType;
}

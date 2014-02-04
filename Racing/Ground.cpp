#include "stdafx.h"
#include "Ground.h"


Ground::Ground(b2World* world, const b2Vec2& pos, const uint tilewidth, const uint tileheight, const float friction) : BoxGameObject(world), frictionCoeff(friction)
{
	b2BodyDef bodydef;
	bodydef.position.Set(pos.x, pos.y);

	body = world->CreateBody(&bodydef);
	body->SetUserData(this);

	b2PolygonShape* dynamicBox = new b2PolygonShape();
	dynamicBox->SetAsBox(tilewidth / racing::BOX2D_METERS_TO_PIXEL, tileheight / racing::BOX2D_METERS_TO_PIXEL);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = dynamicBox;
	fixtureDef.filter.categoryBits = BoxGameObject::GROUND_MASK;

	//actives la detection des collisions seulement pour les roues des voitures
	fixtureDef.filter.maskBits = BoxGameObject::WHEEL_MASK;

	// la voiture passe au travers
	fixtureDef.isSensor = true;

	body->CreateFixture(&fixtureDef);

	delete dynamicBox;
}


Ground::~Ground(void)
{
}

const GameObjectTypes Ground::getGType(void) const
{
	return GameObjectTypes::GroundType;
}

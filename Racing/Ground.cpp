#include "stdafx.h"
#include "Ground.h"


Ground::Ground(b2World* world, const b2Vec2& pos, const uint tilewidth, const uint tileheight, const float friction) : BoxGameObject(world), frictionCoeff(friction)
{
	b2BodyDef bodydef;
	auto halfwidth = tilewidth / racing::BOX2D_METERS_TO_PIXEL / 2;
	auto halfheight = tileheight / racing::BOX2D_METERS_TO_PIXEL / 2;
	bodydef.position.Set(pos.x + halfwidth, pos.y + halfheight);

	body = world->CreateBody(&bodydef);
	body->SetUserData(this);

	b2PolygonShape* dynamicBox = new b2PolygonShape();

	dynamicBox->SetAsBox(halfwidth, halfheight);
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

Ground::Ground(b2World* world, const b2Vec2& pos, std::vector<std::vector<b2Vec2>>& polygons, const uint tilewidth, const uint tileheight, const float friction) : BoxGameObject(world), frictionCoeff(friction)
{
	b2BodyDef bodydef;
	auto halfwidth = tilewidth / racing::BOX2D_METERS_TO_PIXEL / 2;
	auto halfheight = tileheight / racing::BOX2D_METERS_TO_PIXEL / 2;
	bodydef.position.Set(pos.x /*+ halfwidth*/, pos.y /*+ halfheight*/);

	body = world->CreateBody(&bodydef);
	body->SetUserData(this);
	for (auto& points : polygons)
	{
		b2PolygonShape* dynamicBox = new b2PolygonShape();

		dynamicBox->Set(&points[0], points.size());
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
	
}


Ground::~Ground(void)
{
}

const GameObjectTypes Ground::getGType(void) const
{
	return GameObjectTypes::GroundType;
}

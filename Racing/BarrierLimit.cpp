#include "stdafx.h"
#include "BarrierLimit.h"


BarrierLimit::BarrierLimit(b2World* world, float width, float height, float x, float y) : 
BoxGameObject(world),
sf::Drawable(),
rect(x, y, width, height),
rectshape( { width, height } )
{

	auto halfwidth = width / racing::BOX2D_METERS_TO_PIXEL / 2;
	auto halfheight = height / racing::BOX2D_METERS_TO_PIXEL / 2;

	rectshape.setPosition(x, y);

	b2BodyDef bodydef;
	bodydef.type = b2_staticBody;
	bodydef.position.Set(x / racing::BOX2D_METERS_TO_PIXEL + halfwidth, y / racing::BOX2D_METERS_TO_PIXEL + halfheight);

	body = world->CreateBody(&bodydef);
	body->SetUserData(this);

	b2PolygonShape* dynamicBox = new b2PolygonShape();
	dynamicBox->SetAsBox(halfwidth, halfheight);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = dynamicBox;
	fixtureDef.filter.categoryBits = BoxGameObject::BARRIERLIMIT_MASK;
	fixtureDef.filter.maskBits = BoxGameObject::CAR_MASK;
	body->CreateFixture(&fixtureDef);
	delete dynamicBox;
}


BarrierLimit::~BarrierLimit()
{
}


void BarrierLimit::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(rectshape, states);
}

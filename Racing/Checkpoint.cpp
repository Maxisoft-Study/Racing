#include "stdafx.h"
#include "Checkpoint.h"


Checkpoint::Checkpoint(b2World *world, const float init_pos_x, const float init_pos_y, const sf::Vector2u size) :
BoxGameObject(world)
{
	b2BodyDef bodydef;
	bodydef.position.Set(init_pos_x, init_pos_y);

	body = world->CreateBody(&bodydef);
	body->SetUserData(this);

	b2PolygonShape *dynamicBox = new b2PolygonShape();
	dynamicBox->SetAsBox(size.x, size.y);

	b2FixtureDef *fixtureDef = new b2FixtureDef();
	fixtureDef->shape = dynamicBox;
	fixtureDef->filter.categoryBits = BoxGameObject::CHECKPOINT_MASK;

	//actives la detection des collisions seulement pour les voiture
	fixtureDef->filter.maskBits = BoxGameObject::CAR_MASK;

	// la voiture passe au travers
	fixtureDef->isSensor = true;


	body->CreateFixture(fixtureDef);

	delete fixtureDef;
	delete dynamicBox;
}


Checkpoint::~Checkpoint()
{
}


const GameObject::GameObjectTypes Checkpoint::getGType(void) const
{
	return GameObject::CheckpointType;
}

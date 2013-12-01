#include "stdafx.h"
#include "Car.h"

Car::Car(b2World *world, const std::string &file) : SpriteGameObject(file), BoxGameObject(world)
{
	bodydef->type = b2_dynamicBody;
	bodydef->linearDamping = 0.7f;
	bodydef->angularDamping = 0.85;
	bodydef->position.Set(0.f, 0.f);
	body = world->CreateBody(bodydef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(0.5f, 1.0f);

	b2FixtureDef *fixtureDef = new b2FixtureDef();
	fixtureDef->shape = &dynamicBox;
	fixtureDef->density = 50.f;
	fixtureDef->friction = 0.5f;
	body->CreateFixture(fixtureDef);
}



Car::~Car(void)
{
}


void Car::move(const sf::Vector2f vec)
{
	base_sprite.move(vec);
}

void Car::rotate(const float f)
{
	base_sprite.rotate(f);
}
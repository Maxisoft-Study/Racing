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
        dynamicBox.SetAsBox(3.01388888889f, 1.33333333333f);

        b2FixtureDef *fixtureDef = new b2FixtureDef();
        fixtureDef->shape = &dynamicBox;
        fixtureDef->density = 50.f;
        fixtureDef->friction = 0.5f;
        body->CreateFixture(fixtureDef);
}



Car::~Car(void)
{
}


void Car::update(float delta)
{
    auto position = getBody()->GetWorldCenter();
    base_sprite.setPosition(position.x*72.f, position.y*72.f);
    auto angle = getBody()->GetAngle();
    base_sprite.setRotation(angle * 180.f/3.14159265358979f);
}



void Car::move(const sf::Vector2f vec)
{
        base_sprite.move(vec);
}

void Car::rotate(const float f)
{
        base_sprite.rotate(f);
}

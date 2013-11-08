#include "stdafx.h"
#include "Car.h"

Car::Car(sf::Vector2u vector, const std::string &file) : SpriteGameObject(vector, file), velocity(sf::Vector2f()), acceleration(sf::Vector2f())
{
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
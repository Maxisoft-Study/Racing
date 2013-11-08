#pragma once
#include "SpriteGameObject.h"

class Car :
	public SpriteGameObject
{
	friend class sf::RenderTarget;
public:
	Car(sf::Vector2u vector, const std::string &file);
	~Car(void);
	void move(const sf::Vector2f vect);
	void rotate(const float f);
private:
	unsigned short weight;
	unsigned short base_acceleration;
	unsigned short base_speed;
	float handiness;
	sf::Vector2f velocity;
	sf::Vector2f acceleration;
};


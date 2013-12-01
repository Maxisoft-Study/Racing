#pragma once
#include "SpriteGameObject.h"
#include "BoxGameObject.h"

class Car :
	public SpriteGameObject,
	public BoxGameObject
{
	friend class sf::RenderTarget;
public:
	Car(b2World *world, const std::string &file);
	~Car(void);

	void move(const sf::Vector2f vect);
	void rotate(const float f);
private:
	unsigned short weight;
	unsigned short base_acceleration;
	unsigned short base_speed;
	float handiness;
};


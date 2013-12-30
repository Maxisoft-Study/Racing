#pragma once
#include "stdafx.h"
#include "MixedGameObject.h"

class Car;

#include "Wheel.h"

class CarControl
{
public:
	CarControl::CarControl(const char dir = 0, const char rot = 0) : rotation(rot), direction(dir){};
	CarControl::~CarControl(void){};
	char rotation;
	char direction;
};

class Car :
	public MixedGameObject
{
	friend class sf::RenderTarget;
	friend class CarControler;
	friend class Wheel;
public:
	Car(b2World *world, const std::string &file, const float init_pos_x = 0.f, const float init_pos_y = 0.f);
	~Car(void);
	static b2Vec2 DIM;
	virtual void update(float delta) final;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
private:
	unsigned short weight;
	unsigned short base_acceleration;
	unsigned short base_speed;
	float handiness;
	float maxfrontwheelsangle;
	CarControl lastcontrol;
	std::vector<Wheel*> wheels;
	std::vector<b2Joint*> wheelsJoints;
};


#pragma once
#include "stdafx.h"
#include "MixedGameObject.h"

class Car;

#include "Wheel.h"
#include "CarEngine.h"

class CarControl
{
public:
	CarControl::CarControl(const float dir = 0.f, const float rot = 0.f) : rotation(rot), direction(dir){};
	CarControl::~CarControl(void){};
	float rotation;
	float direction;
};

enum CarSide
{
	ahead,
	back
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
	virtual void update(float delta) final;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
	const float getSpeed(void) const;
	const CarControl getlastControl() const;
	const CarSide getSide() const;
private:
	unsigned short weight;
	unsigned short base_acceleration;
	unsigned short base_speed;
	float handiness;
	float maxfrontwheelsangle;
	float lastspeed;
	CarEngine *engine;
	CarControl lastcontrol;
	std::vector<Wheel*> wheels;
	std::vector<b2Joint*> wheelsJoints;
	void applyInertia(const float delta);
};


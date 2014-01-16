#pragma once
#include "GameObject.h"
class CarEngine;

#include "Car.h"

class CarEngine :
	public GameObject
{
	friend class Car;
public:
	CarEngine(Car* carp);
	~CarEngine();
	float getBaseImpulseY(void);
private:
	Car *car_ptr;
	int speed_pad;
};


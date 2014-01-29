#pragma once
class CarEngine;

#include "Car.h"

class CarEngine
{
	friend class Car;
public:
	explicit CarEngine(Car* carp);
	~CarEngine();
	float getBaseImpulseY(void);
	int getSpeedPad(void) const;
private:
	Car* car_ptr;
	int speed_pad;
};


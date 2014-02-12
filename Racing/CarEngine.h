#pragma once
#include "stdafx.h"
class Car;

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


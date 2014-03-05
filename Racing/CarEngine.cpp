#include "stdafx.h"
#include "CarEngine.h"
#include "Car.h"

CarEngine::CarEngine(Car *carp) : car_ptr(carp), speed_pad(0)
{
}


CarEngine::~CarEngine()
{
}



float CarEngine::getBaseImpulseY(void)
{
	// Speed calibration
	const float speed = car_ptr->getSpeed();
	const float speed2 = speed * speed;
	const float speed3 = speed2 * speed;
	if (car_ptr->getlastControl().direction == 1) // marche arrière
	{
		if (car_ptr->getSide() == CarSide::Ahead) // freinage
		{
			speed_pad = static_cast<int>(2.02E-06f * speed3 + -5.53E-04f * speed2 + 7.09E-02f * speed);
			return -6.93E-06f*speed3 + 2.56E-03f*speed2 + 2.33E-01f*speed + 3.99E+01f;
		}
		//else marche arrière
		speed_pad = -1;
		return 25;
		
	}
	speed_pad = static_cast<int>(2.02E-06f * speed3 + -5.53E-04f * speed2 + 7.09E-02f * speed);
	return -6.93E-06f*speed3 + 2.56E-03f*speed2 + 2.33E-01f*speed + 3.99E+01f;
}


int CarEngine::getSpeedPad(void) const
{
	return speed_pad;
}

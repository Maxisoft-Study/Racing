#pragma once
#include "stdafx.h"
#include "Car.h"

struct CarControlDef
{
	sf::Keyboard::Key MoveForward;
	sf::Keyboard::Key MoveBackWard;
	sf::Keyboard::Key RotateLeft;
	sf::Keyboard::Key RotateRight;

	CarControlDef(sf::Keyboard::Key moveForward = sf::Keyboard::Key::Up,
		sf::Keyboard::Key moveBackWard = sf::Keyboard::Key::Down,
		sf::Keyboard::Key rotateLeft = sf::Keyboard::Key::Left,
		sf::Keyboard::Key rotateRight = sf::Keyboard::Key::Right) :
		MoveForward(moveForward), MoveBackWard(moveBackWard), RotateLeft(rotateLeft), RotateRight(rotateRight)
	{}
};

class CarControler
{
public:
	CarControler(Car *car, const CarControlDef carcontroldef) : car_ptr(car), controldef(carcontroldef){}
	virtual bool parseKeys()
	{
		float dir = 0.f;
		float rot = 0.f;
		if (sf::Keyboard::isKeyPressed(controldef.RotateLeft))
		{
			rot -= 1.f;
		}
		if (sf::Keyboard::isKeyPressed(controldef.MoveForward))
		{
			dir -= 1.f;
		}
		if (sf::Keyboard::isKeyPressed(controldef.RotateRight))
		{
			rot += 1.f;
		}
		if (sf::Keyboard::isKeyPressed(controldef.MoveBackWard))
		{
			dir += 1.f;
		}

		car_ptr->lastcontrol = { dir, rot };

		return dir || rot;
	}
protected:
private:
	Car *car_ptr;
	const CarControlDef controldef;
};

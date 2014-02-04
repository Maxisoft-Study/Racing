#pragma once
#include "stdafx.h"
#include "Car.h"

struct CarControlKeysDef
{
	sf::Keyboard::Key MoveForward;
	sf::Keyboard::Key MoveBackWard;
	sf::Keyboard::Key RotateLeft;
	sf::Keyboard::Key RotateRight;

	explicit CarControlKeysDef(sf::Keyboard::Key moveForward = sf::Keyboard::Key::Up,
		sf::Keyboard::Key moveBackWard = sf::Keyboard::Key::Down,
		sf::Keyboard::Key rotateLeft = sf::Keyboard::Key::Left,
		sf::Keyboard::Key rotateRight = sf::Keyboard::Key::Right) :
		MoveForward(moveForward), MoveBackWard(moveBackWard), RotateLeft(rotateLeft), RotateRight(rotateRight)
	{}
};

class CarControler
{
public:
	CarControler(Car *car, const CarControlKeysDef carcontrolkeysdef) : car_ptr(car), keysdef(carcontrolkeysdef){}
	virtual bool parseKeys()
	{
		float dir = 0.f;
		float rot = 0.f;
		if (sf::Keyboard::isKeyPressed(keysdef.RotateLeft))
		{
			rot -= 1.f;
		}
		if (sf::Keyboard::isKeyPressed(keysdef.MoveForward))
		{
			dir -= 1.f;
		}
		if (sf::Keyboard::isKeyPressed(keysdef.RotateRight))
		{
			rot += 1.f;
		}
		if (sf::Keyboard::isKeyPressed(keysdef.MoveBackWard))
		{
			dir += 1.f;
		}

		car_ptr->lastcontrol = { dir, rot };

		return dir || rot;
	}
protected:
private:
	Car *car_ptr;
	const CarControlKeysDef keysdef;
};

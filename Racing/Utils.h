#pragma once
#include "stdafx.h"

class Utils
{
public:
	static b2Vec2 RotateVect(const b2Vec2 vect, const float angle);
	static sf::Vector2f RotateVect(const sf::Vector2f vect, const float angle);
	static float RadianToDegree(const float angle);
	static float DegreeToRadian(const float angle);
	static sf::Vector2f Box2DVectToSfVectPixel(const b2Vec2 v);
private:
	Utils();
	~Utils();
};

static const float PI = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348;
static const float TWOPI = 6.2831853071795864769252867665590057683943387987502116419498891846156328125724179972560696;


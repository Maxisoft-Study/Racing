#include "stdafx.h"
#include "Utils.h"

Utils::Utils()
{
}


Utils::~Utils()
{
}



b2Vec2 Utils::RotateVect(const b2Vec2 vect, const float angle)
{
	/*float cosa = std::cosf(angle);
	float sina = std::sinf(angle);

	return b2Vec2(cosa*vect.x - sina * vect.y, sina * vect.x + cosa * vect.y);*/
	
	return b2Mul(b2Rot(angle), vect);

}

sf::Vector2f Utils::RotateVect(const sf::Vector2f vect, const float angle)
{
	float cosa = std::cosf(angle);
	float sina = std::sinf(angle);

	return sf::Vector2f(cosa* vect.x - sina * vect.y, sina * vect.x + cosa * vect.y);
}


float Utils::RadianToDegree(const float angle)
{
	return angle * 180.f / PI;
}

float Utils::DegreeToRadian(const float angle)
{
	return angle * PI / 180.f;
}


sf::Vector2f Utils::Box2DVectToSfVectPixel(const b2Vec2 v)
{
	return sf::Vector2f(v.x * racing::BOX2D_METERS_TO_PIXEL, v.y * racing::BOX2D_METERS_TO_PIXEL);
}

b2Vec2 Utils::SfVectPixelToBox2DVect(const sf::Vector2f v)
{
	return b2Vec2(v.x / racing::BOX2D_METERS_TO_PIXEL, v.y / racing::BOX2D_METERS_TO_PIXEL);
}



#include "stdafx.h"
#include "Utils.h"


Utils::Utils()
{
}


Utils::~Utils()
{
}



b2Vec2 Utils::rotateVect(const b2Vec2 vect, const float angle){
	float cosa = std::cosf(angle);
	float sina = std::sinf(angle);

	return b2Vec2(cosa*vect.x - sina * vect.y, sina * vect.x + cosa * vect.y);
}

sf::Vector2f Utils::rotateVect(const sf::Vector2f vect, const float angle){
	float cosa = std::cosf(angle);
	float sina = std::sinf(angle);

	return sf::Vector2f(cosa* vect.x - sina * vect.y, sina * vect.x + cosa * vect.y);
}

b2Vec2 Utils::rotatePoint(const b2Vec2 vect, const float angle){
	return b2Vec2(vect.x * -std::sinf(angle), vect.y * std::cosf(angle));
}

#pragma once
#include "stdafx.h"

class Utils
{
public:
	static b2Vec2 rotateVect(const b2Vec2 vect, const float angle);
	static sf::Vector2f rotateVect(const sf::Vector2f vect, const float angle);
	static b2Vec2 rotatePoint(const b2Vec2 vect, const float angle);
private:
	Utils();
	~Utils();
};


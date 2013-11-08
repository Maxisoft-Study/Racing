#pragma once
#include "stdafx.h"
#include "Car.h"
#include "Obstacle.h"

enum CollisionArrayType : unsigned char
{
	OBSTACLE,
	MOVABLE,
	count
};

struct collisionMatrixElement
{
	CollisionArrayType type;
	unsigned short index;
};

class CollisionHandler
{
private:
	std::vector<Obstacle*> obstacles; // taille max = 2^16 - 1
	std::vector<Car*> cars; // taille max = 2^16 - 1
	std::vector<std::vector<collisionMatrixElement> > collision_matrix;
public:
	CollisionHandler(void);
	~CollisionHandler(void);
	bool update(void);
};


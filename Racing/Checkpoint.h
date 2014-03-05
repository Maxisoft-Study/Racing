#pragma once
#include "BoxGameObject.h"
class Checkpoint :
	public BoxGameObject
{
public:
	Checkpoint(b2World* world, const std::string nameparam, const float init_pos_x, const float init_pos_y, const sf::Vector2u size);
	~Checkpoint(void);
	const virtual GameObjectTypes getGType(void) const;

	inline const std::string getName() const{
		return name;
	}
private:
	std::string name;
};


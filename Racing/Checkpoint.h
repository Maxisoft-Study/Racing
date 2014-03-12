#pragma once
#include "BoxGameObject.h"
class Checkpoint :
	public BoxGameObject
{
public:
	Checkpoint(b2World* world, const std::string nameparam, const float init_pos_x, const float init_pos_y, const sf::Vector2u size);
	~Checkpoint(void);
	const virtual GameObjectTypes getGType(void) const final;

	inline const std::string& getName() const{
		return name;
	}

	bool operator<(const Checkpoint& c)const{ return name.compare(c.getName()) < 0; }

private:
	std::string name;
};


struct CheckpointPtrComp
{
	bool operator()(const Checkpoint* lhs, const Checkpoint* rhs) const  { return lhs->getName().compare(rhs->getName()) < 0; }
};


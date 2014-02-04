#pragma once
class Ground : public BoxGameObject
{
public:
	Ground(b2World* world, const b2Vec2& pos, const uint tilewidth, const uint tileheight, const float friction = 1.f);
	~Ground(void);
	virtual const GameObjectTypes getGType(void) const;
private:
    float frictionCoeff;
};

#pragma once
#include "SpriteGameObject.h"
#include "BoxGameObject.h"

class MixedGameObject :
	public SpriteGameObject,
	public BoxGameObject
{
	friend class sf::RenderTarget;
public:
	using BoxGameObject::getGType;
	~MixedGameObject();
	virtual void update(float delta);
protected:
	using BoxGameObject::gtype;
	using BoxGameObject::setGType;
	MixedGameObject(b2World *world, const std::string &file, const float init_pos_x = 0.f, const float init_pos_y = 0.f);
};


#pragma once
#include "SpriteGameObject.h"
#include "BoxGameObject.h"

class MixedGameObject :
	public SpriteGameObject,
	public BoxGameObject
{
	friend class sf::RenderTarget;
public:
	~MixedGameObject();
	virtual void update(float delta);
	const virtual GameObjectTypes getGType(void) const;
protected:
	MixedGameObject(b2World* world, const std::string& file);
};


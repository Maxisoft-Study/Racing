#pragma once
#include "SpriteGameObject.h"
#include "BoxGameObject.h"

class MixedGameObject :
	public SpriteGameObject,
	public BoxGameObject,
	public virtual Updatable
{
	friend class sf::RenderTarget;
public:
	~MixedGameObject();
	virtual void update(float delta);
	const virtual GameObjectTypes getGType(void) const;
	inline bool getRoundCoordinate() const { return roundCoordinate; }
	inline void setRoundCoordinate(bool roundc){ roundCoordinate = roundc; }
protected:
	MixedGameObject(b2World* world, const std::string& file);
private:
	bool roundCoordinate;
};


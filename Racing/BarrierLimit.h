#pragma once
#include "stdafx.h"
#include "BoxGameObject.h"
class BarrierLimit :
	public BoxGameObject,
	virtual public sf::Drawable
{
	friend class sf::RenderTarget;
public:
	BarrierLimit(b2World* world, float width, float height, float x, float y);
	~BarrierLimit();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
	inline const sf::Rect<float> getRect(void) const { return rect; }
	const virtual GameObjectTypes getGType(void) const final
	{
		return GameObjectTypes::BarrierLimitType;
	}
private:
	sf::Rect<float> rect;
	sf::RectangleShape rectshape;
};


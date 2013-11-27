#pragma once
#include "stdafx.h"
#include "GameObject.h"
class SpriteGameObject :
	public GameObject,
	public sf::Drawable
{
public:
	virtual void update(float delta) {}; //TODO
protected:
	sf::Sprite base_sprite;
	sf::Texture texture;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	SpriteGameObject(sf::Vector2u vector, const std::string &file, bool center=true);
	~SpriteGameObject(void);
private:
};


#pragma once
#include "stdafx.h"
#include "GameObject.h"
class SpriteGameObject :
	public GameObject,
	public sf::Drawable
{

friend class sf::RenderTarget;
public:
	virtual void update(float delta) {}; //TODO
	SpriteGameObject(const std::string &file, bool center = true);
	~SpriteGameObject(void);

	void moveSprite(const sf::Vector2f vec)
	{
		base_sprite.move(vec);
	}

	void rotateSprite(const float f)
	{
		base_sprite.rotate(f);
	}

	void setSpriteRotation(const float f)
	{
		base_sprite.setRotation(f);
	}

	void setSpritePosition(const sf::Vector2f vec)
	{
		base_sprite.setPosition(vec);
	}
	void setSpritePosition(const float x, const float y)
	{
		base_sprite.setPosition(x, y);
	}

protected:
	sf::Sprite base_sprite;
	sf::Texture texture;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
};

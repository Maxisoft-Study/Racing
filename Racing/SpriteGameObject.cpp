#include "stdafx.h"
#include "SpriteGameObject.h"


SpriteGameObject::SpriteGameObject(sf::Vector2u vector, const std::string &file, bool auto_center) : GameObject(vector)
{
	texture.loadFromFile(file);
	texture.setSmooth(true);
	base_sprite.setTexture(texture);
	if (auto_center)
	{
		sf::Vector2u tmp_texture_size = texture.getSize();
		base_sprite.setOrigin( tmp_texture_size.x / 2.f, tmp_texture_size.y / 2.f ) ; // Centre pour futures transformation
	}
	
}


SpriteGameObject::~SpriteGameObject(void)
{
}


void SpriteGameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(base_sprite, states);
}

#include "stdafx.h"
#include "SpriteGameObject.h"


SpriteGameObject::SpriteGameObject(const std::string &file, bool auto_center) : GameObject()
{
	texture = TextureLoader::Instance().get(file);
    //texture.setSmooth(true);
    base_sprite.setTexture(*texture);
    if (auto_center)
    {
		sf::Vector2u tmp_texture_size = texture->getSize();
		base_sprite.setOrigin( tmp_texture_size.x / 2.f, tmp_texture_size.y / 2.f ) ; // Centre pour futures transformations
    }
	
}


SpriteGameObject::~SpriteGameObject(void)
{
}


void SpriteGameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = texture.get();
    target.draw(base_sprite, states);
}


sf::Vector2u SpriteGameObject::getImageSize(void) const
{
	return texture->getSize();
}

/*const GameObject::GameObjectTypes SpriteGameObject::getGType(void) const
{
	return GameObject::SpriteGameObjectType;
}*/

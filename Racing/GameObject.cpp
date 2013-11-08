#include "stdafx.h"
#include "GameObject.h"


GameObject::GameObject( sf::Vector2u vector) : size(vector), position(0.f,0.f)
{

}

GameObject::~GameObject(void)
{
}

const sf::Vector2i GameObject::getPositionInt()
{
	return sf::Vector2i(position); //static cast
}
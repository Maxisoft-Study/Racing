#include "stdafx.h"
#include "Level.h"


Level::Level(void)
{
}


Level::~Level(void)
{
}


void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	for (TileMap tm : tilemaps)
	{
		target.draw(tm, states);
	}
}

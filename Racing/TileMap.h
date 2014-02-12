#pragma once
#include "stdafx.h"
#include "TextureLoader.h"


//////////////////////////////////////////////////////////////////////////
/// basée sur http://www.sfml-dev.org/tutorials/2.1/graphics-vertex-array-fr.php
//////////////////////////////////////////////////////////////////////////
class TileMap : public sf::Drawable, public sf::Transformable
{
public:
	bool load(const std::string& tileset, sf::Vector2u tileSize, const std::vector<int>& tiles, uint width, uint height, int min_val, int max_val, uint spacing = 0);
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
	sf::VertexArray m_vertices;
	texture_ptr m_tileset;
};
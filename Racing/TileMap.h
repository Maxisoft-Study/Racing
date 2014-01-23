#pragma once
#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
/// basée sur http://www.sfml-dev.org/tutorials/2.1/graphics-vertex-array-fr.php
//////////////////////////////////////////////////////////////////////////
class TileMap : public sf::Drawable, public sf::Transformable
{
public:

	bool load(const std::string& tileset, sf::Vector2u tileSize, const std::vector<uint> &tiles, uint width, uint height, uint min_val, uint max_val, uint spacing = 0)
	{
		// on charge la texture du tileset
		if (!m_tileset.loadFromFile(tileset))
			return false;

		// on redimensionne le tableau de vertex pour qu'il puisse contenir tout le niveau
		m_vertices.setPrimitiveType(sf::Quads);
		m_vertices.resize(width * height * 4);

		// on remplit le tableau de vertex, avec un quad par tuile
		for (unsigned int i = 0; i < width; ++i)
		for (unsigned int j = 0; j < height; ++j)
		{
			// on récupère le numéro de tuile courant
			const uint tileIndex = i + j * width;
			uint tileNumber = tiles[tileIndex];

			if (tileNumber < min_val || tileNumber > max_val)
			{
				continue;
			}

			tileNumber -= min_val;

			// on en déduit sa position dans la texture du tileset
			int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
			int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

			// on récupère un pointeur vers le quad à définir dans le tableau de vertex
			sf::Vertex* quad = &m_vertices[(tileIndex)* 4];

			// on définit ses quatre coins
			quad[0].position = sf::Vector2f(static_cast<float>(i * tileSize.x), static_cast<float>(j * tileSize.y));
			quad[1].position = sf::Vector2f(static_cast<float>((i + 1) * tileSize.x), static_cast<float>(j * tileSize.y));
			quad[2].position = sf::Vector2f(static_cast<float>((i + 1) * tileSize.x), static_cast<float>((j + 1) * tileSize.y));
			quad[3].position = sf::Vector2f(static_cast<float>(i * tileSize.x), static_cast<float>((j + 1) * tileSize.y));

			// on définit ses quatre coordonnées de texture
			float spacingx = static_cast<float>(tu * spacing);
			float spacingy = static_cast<float>(tv * spacing);
			quad[0].texCoords = sf::Vector2f(tu * tileSize.x + spacingx, tv * tileSize.y + spacingy);
			quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x + spacingx, tv * tileSize.y + spacingy);
			quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x + spacingx, (tv + 1) * tileSize.y + spacingy);
			quad[3].texCoords = sf::Vector2f(tu * tileSize.x + spacingx, (tv + 1) * tileSize.y + spacingy);
		}

		return true;
	}

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// on applique la transformation
		states.transform *= getTransform();

		// on applique la texture du tileset
		states.texture = &m_tileset;

		// et on dessine enfin le tableau de vertex
		target.draw(m_vertices, states);
	}

	sf::VertexArray m_vertices;
	sf::Texture m_tileset;
};
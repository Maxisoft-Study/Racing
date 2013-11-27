#pragma once
#include "stdafx.h"
#include "Ground.h"

class Level :
	public sf::Drawable,
	public sf::Transformable
{
	friend class sf::RenderTarget;
private:
	sf::VertexArray vertices;
	sf::Texture tileset;
	std::vector<std::vector<Ground*> > grounds; // TODO singleton ground
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const; //TODO http://www.sfml-dev.org/tutorials/2.1/graphics-vertex-array-fr.php

public:
	Level(void);
	~Level(void);
};


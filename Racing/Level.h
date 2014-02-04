#pragma once
#include "stdafx.h"
#include "Ground.h"
#include "Checkpoint.h"
#include "Ground.h"

struct TileSetDef
{
	std::string image;
	uint firstgid;
	uint imageheight;
	uint imagewidth;
	uint spacing;
	uint tileheight;
	uint tilewidth;

	TileSetDef(std::string img, uint firstgidparam, uint iwidth, uint iheight, uint twidth, uint theight, uint spacingparam) :
		image(img), firstgid(firstgidparam), imageheight(iheight), imagewidth(iwidth), spacing(spacingparam), tilewidth(twidth), tileheight(theight)
	{}

	TileSetDef& operator =(const TileSetDef &other)
	{
		image = other.image;
		firstgid = other.firstgid;
		imageheight = other.imageheight;
		imagewidth = other.imagewidth;
		spacing = other.spacing;
		tileheight = other.tileheight;
		tilewidth = other.tilewidth;
		return *this;
	}
};

class Level :
    virtual public sf::Drawable
{
private:

	//////////////////////////////////////////////////////////////////////////
	/// Les tableau de tuiles
	//////////////////////////////////////////////////////////////////////////
	std::vector<TileMap> tilemaps;

	//////////////////////////////////////////////////////////////////////////
	/// les checkpoints
	//////////////////////////////////////////////////////////////////////////
	std::vector<Checkpoint*> checkpoints;

	//////////////////////////////////////////////////////////////////////////
	/// La definition des tilesets
	//////////////////////////////////////////////////////////////////////////
	std::vector<const TileSetDef> tilesetdefs;

	boost::numeric::ublas::matrix<Ground*> groundmatrix;


	//////////////////////////////////////////////////////////////////////////
	/// Taille de base des tuiles
	//////////////////////////////////////////////////////////////////////////
	sf::Vector2u tileLenght;

	//////////////////////////////////////////////////////////////////////////
	/// taile en nombre de tuiles
	//////////////////////////////////////////////////////////////////////////
	sf::Vector2u lenght;

public:

	Level(void);
	~Level(void);

	bool load(const std::string& jsonfilename, b2World* world);

	const sf::Vector2u getRawLenght() const { return lenght; }
	const sf::Vector2u getLenght() const { return sf::Vector2u(lenght.x * tileLenght.x, lenght.y * tileLenght.y); }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	const TileSetDef* searchForTileSetDef(const int id) const;
};

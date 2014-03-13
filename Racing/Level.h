#pragma once
#include "stdafx.h"
#include "Ground.h"
#include "Checkpoint.h"
#include "Ground.h"
#include "BarrierLimit.h"
#include "StartPos.h"

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
	/// Les barrières
	//////////////////////////////////////////////////////////////////////////
	std::vector<BarrierLimit*> barriers;

	//////////////////////////////////////////////////////////////////////////
	/// les checkpoints
	//////////////////////////////////////////////////////////////////////////
	std::set<Checkpoint*, CheckpointPtrComp> checkpoints;

	//////////////////////////////////////////////////////////////////////////
	/// La definition des tilesets
	//////////////////////////////////////////////////////////////////////////
	std::vector<const TileSetDef> tilesetdefs;

	boost::numeric::ublas::matrix<std::vector<Ground*>> groundmatrix;


	//////////////////////////////////////////////////////////////////////////
	/// Taille de base des tuiles
	//////////////////////////////////////////////////////////////////////////
	sf::Vector2u tileLenght;

	//////////////////////////////////////////////////////////////////////////
	/// taille en nombre de tuiles
	//////////////////////////////////////////////////////////////////////////
	sf::Vector2u lenght;

	//////////////////////////////////////////////////////////////////////////
	/// les positions de départ
	//////////////////////////////////////////////////////////////////////////
	std::vector<const StartPos> startPos;

	std::unordered_map<int, boost::filesystem::path> listYmlGroundDefFiles(boost::filesystem::path& p) const;

public:

	Level(void);
	~Level(void);

	bool load(const std::string& jsonfilename, b2World* world, boost::filesystem::path& p=boost::filesystem::path("./"));
	
	const Checkpoint* getFirstCheckpoint() const;
	const Checkpoint* getNextCheckpoint(Checkpoint* cp) const;

	inline const sf::Vector2u getRawLenght() const { return lenght; }
	inline const sf::Vector2u getLenght() const { return sf::Vector2u(lenght.x * tileLenght.x, lenght.y * tileLenght.y); }
	inline const StartPos& getStartPos(const int i) const { return startPos[i]; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	const TileSetDef* searchForTileSetDef(const int id) const;
};

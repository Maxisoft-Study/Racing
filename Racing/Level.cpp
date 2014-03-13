#include "stdafx.h"
#include "Level.h"

//JSON
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

Level::Level(void)
{
}


Level::~Level(void)
{
	for (auto checkpoint : checkpoints)
	{
		delete checkpoint;
	}

	for (unsigned i = 0, k = 0; i < groundmatrix.size1(); ++i)
		for (unsigned j = 0; j < groundmatrix.size2(); ++j)
		{
			auto ground = groundmatrix(j, i);
			if (ground)
			{
				delete ground;
			}
		}

	for (auto barrier : barriers)
	{
		delete barrier;
	}
}


void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto& tm : tilemaps)
	{
		target.draw(tm, states);
	}

	for (auto barrier : barriers)
	{
		target.draw(*barrier, states);
	}

}

bool Level::load(const std::string &jsonfilename, b2World* world, boost::filesystem::path& path)
{

	boost::property_tree::ptree pt;
	boost::property_tree::read_json(jsonfilename, pt);

	// taile en nombre de tuiles
	lenght = sf::Vector2u(pt.get<uint>("width"), pt.get<uint>("height"));

	std::vector<std::vector<int>> tilemapdata;
	int tilelayer = 0;

	//Parse 
	for (auto &layers : pt.get_child("layers"))
	{
		std::string layertype = layers.second.get<std::string>("type");


		if (layertype == "tilelayer") // Parse to tile map
		{
			tilemapdata.emplace_back();
			for (auto &rawdata : layers.second.get_child("data"))
			{
				tilemapdata[tilelayer].emplace_back(rawdata.second.get<int>(""));
			}
			tilelayer += 1;
		}


		if (layertype == "objectgroup") // Parse to GameObject
		{
			for (auto &rawobject : layers.second.get_child("objects"))
			{
				auto jsonobject = rawobject.second;
				std::string objecttype = jsonobject.get<std::string>("type");

				if (objecttype == "checkpoint")
				{
					Checkpoint *newcp = new Checkpoint(
						world,
						jsonobject.get<std::string>("name"),
						jsonobject.get<int>("x") / racing::BOX2D_METERS_TO_PIXEL,
						jsonobject.get<int>("y") / racing::BOX2D_METERS_TO_PIXEL,
						sf::Vector2u(static_cast<uint>(jsonobject.get<uint>("width") / racing::BOX2D_METERS_TO_PIXEL),
							static_cast<uint>(jsonobject.get<uint>("height") / racing::BOX2D_METERS_TO_PIXEL)));

					checkpoints.emplace(newcp);
				}
				else if (objecttype == "start")
				{
					startPos.emplace_back(
						jsonobject.get<int>("x") / racing::BOX2D_METERS_TO_PIXEL,
						jsonobject.get<int>("y") / racing::BOX2D_METERS_TO_PIXEL,
						jsonobject.get_child("properties").get<float>("angle")
						);
				}
				else if (objecttype == "limit")
				{
					auto barriere = new BarrierLimit(
						world,
						jsonobject.get<float>("width"),
						jsonobject.get<float>("height"),
						jsonobject.get<float>("x"),
						jsonobject.get<float>("y"));
					barriers.push_back(barriere);
				}
			}
		}
	}
	LOG_DEBUG << startPos.size();

	for (auto &tileset : pt.get_child("tilesets"))
	{
		auto jsonobject = tileset.second;
		auto imagepath = jsonobject.get<std::string>("image");
		if (!boost::starts_with(imagepath, "ressources/")){
			imagepath = std::string("ressources/").append(imagepath);
		}
		tilesetdefs.emplace_back(
			imagepath,
			jsonobject.get<uint>("firstgid"),
			jsonobject.get<uint>("imagewidth"),
			jsonobject.get<uint>("imageheight"),
			jsonobject.get<uint>("tilewidth"),
			jsonobject.get<uint>("tileheight"),
			jsonobject.get<uint>("spacing")
			);
	}

	//sort de la liste des tuiles defs en fonction de l'id des tuiles
	std::sort(begin(tilesetdefs), end(tilesetdefs), [](const TileSetDef left, const TileSetDef right) { return left.firstgid < right.firstgid; });

	for (auto layerdata : tilemapdata)
	{
		//creation des tilemaps
		for (auto it = begin(tilesetdefs); it != end(tilesetdefs); ++it)
		{
			TileMap curr;

			uint nextgid = -1;
			auto nextit = it + 1;
			if (nextit != end(tilesetdefs))
			{
				nextgid = nextit->firstgid - 1;
			}

			bool result = curr.load(
				it->image,
				sf::Vector2u(it->tilewidth, it->tileheight),
				layerdata,
				lenght.x,
				lenght.y,
				it->firstgid,
				nextgid,
				it->spacing
				);

			if (!result)
			{
				return false;
			}

			tilemaps.push_back(curr);
		}
	}
	



	//remplir la matrice d'objets ground
	groundmatrix.resize(lenght.x, lenght.y);

	for (unsigned i = 0, k = 0; i < lenght.x; ++i)
		for (unsigned j = 0; j < lenght.y; ++j)
		{
			int data = tilemapdata[1][k];
			const TileSetDef* currDef = searchForTileSetDef(data);


			auto position = Utils::SfVectPixelToBox2DVect({ static_cast<float>(j * currDef->tilewidth), static_cast<float>(i * currDef->tilewidth) });
#ifdef DISABLETERRAIN
			if (data && data != 38)
			{
				groundmatrix(j, i) = new Ground(world, position, currDef->tilewidth, currDef->tileheight);
			}
			else
			{
				groundmatrix(j, i) = nullptr;
			}
#else
			groundmatrix(j, i) = new Ground(world, position, currDef->tilewidth, currDef->tileheight, !data ? 0.95f : 1.f);
#endif // DEBUG_DEBUG_DRAW		
			k += 1;
		}

	return true;
}


const TileSetDef* Level::searchForTileSetDef(const int id) const
{
	for (auto it = begin(tilesetdefs); it != end(tilesetdefs); ++it)
	{
		if (static_cast<uint>(id) < it->firstgid)
		{
			return it == begin(tilesetdefs) ?  it._Ptr : (it - 1)._Ptr;
		}
	}
	return begin(tilesetdefs)._Ptr;
}


const Checkpoint* Level::getNextCheckpoint(Checkpoint* cp) const
{
	auto res (checkpoints.find(cp));
	if (res == end(checkpoints)){
		return nullptr;
	}
	++res;
	if (res == end(checkpoints)){
		return nullptr;
	}
	return *res;
}


const Checkpoint* Level::getFirstCheckpoint() const
{
	const auto ret = checkpoints.begin();
	if (ret == end(checkpoints)){
		return nullptr;
	}
	return *ret;
}
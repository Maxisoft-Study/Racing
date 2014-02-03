#include "stdafx.h"
#include "Level.h"


Level::Level(void)
{
}


Level::~Level(void)
{
	for (auto checkpoint : checkpoints)
	{
		delete checkpoint;
	}
}


void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (TileMap tm : tilemaps)
	{
		target.draw(tm, states);
	}
}

bool Level::load(const std::string &jsonfilename, b2World *world)
{

	boost::property_tree::ptree pt;
	boost::property_tree::read_json(jsonfilename, pt);


	lenght = sf::Vector2u(pt.get<uint>("width"), pt.get<uint>("height"));

	std::vector<int> tilemapdata;

	//Parse 
	for (auto &layers : pt.get_child("layers"))
	{
		std::string layertype = layers.second.get<std::string>("type");


		if (layertype == "tilelayer" && !tilemapdata.size()) // Parse to tile map
		{
			for (auto &rawdata : layers.second.get_child("data"))
			{
				tilemapdata.emplace_back(rawdata.second.get<int>(""));
			}
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
						jsonobject.get<int>("x") / racing::BOX2D_METERS_TO_PIXEL,
						jsonobject.get<int>("y") / racing::BOX2D_METERS_TO_PIXEL,
						sf::Vector2u(static_cast<uint>(jsonobject.get<uint>("width") / racing::BOX2D_METERS_TO_PIXEL),
							static_cast<uint>(jsonobject.get<uint>("height") / racing::BOX2D_METERS_TO_PIXEL)));

					checkpoints.push_back(newcp);
				}
			}
		}
	}

	for (auto &tileset : pt.get_child("tilesets"))
	{
		auto jsonobject = tileset.second;
		
		tilesetdefs.emplace_back(
			jsonobject.get<std::string>("image"),
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
			tilemapdata,
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

	return true;
}

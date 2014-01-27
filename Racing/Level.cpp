#include "stdafx.h"
#include "Level.h"


Level::Level(void)
{
}


Level::~Level(void)
{
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
					checkpoints.emplace_back(
						world,
						jsonobject.get<int>("x") / racing::BOX2D_METERS_TO_PIXEL,
						jsonobject.get<int>("y") / racing::BOX2D_METERS_TO_PIXEL,
						sf::Vector2u(static_cast<uint>(jsonobject.get<uint>("width") / racing::BOX2D_METERS_TO_PIXEL),
							static_cast<uint>(jsonobject.get<uint>("height") / racing::BOX2D_METERS_TO_PIXEL))
						);
				}
			}
		}
	}

	return true;
}

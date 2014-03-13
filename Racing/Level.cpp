#include "stdafx.h"
#include <regex>
#include "Level.h"

//JSON
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::filesystem;

//UTILS RANGE CONTENER
typedef std::pair<int, int> range;
range make_range(int lower, int upper) {
	if (upper < lower) {
		return std::make_pair(upper, lower);
	}
	return std::make_pair(lower, upper);
}



struct rangecmp {
	bool operator()(range const& a, range const& b) {
		if (a.second < b.first) return true;
		return false;
	}
};


typedef std::map<range, TileSetDef*, rangecmp> rangemap;
// END UTILS RANGE CONTENER

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
			auto grounds = groundmatrix(j, i);
			for (auto ground : grounds)
			{
				if (ground)
				{
					delete ground;
				}
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



std::unordered_map<int, boost::filesystem::path> Level::listYmlGroundDefFiles(boost::filesystem::path& p) const
{
	std::unordered_map<int, boost::filesystem::path> ret;
	directory_iterator end_iter;
	for (directory_iterator itr(p); itr != end_iter; ++itr)
	{
		auto fpath = path(*itr);
		auto file = fpath.filename();
		auto filestring = file.string();
		if (std::regex_match(filestring, std::regex("^\\d+\\.y[a]?ml$")))
		{
			std::stringstream os(filestring);
			int val;
			os >> val;
			ret[val] = fpath;
		}
	}
	return ret;
}


bool Level::load(const std::string &jsonfilename, b2World* world, boost::filesystem::path& p)
{
	rangemap tiledefmap;
	auto ymlgroundDef = listYmlGroundDefFiles(p);

	YAML::Node frictionyaml = YAML::LoadFile((p / "friction.yml").string());
	auto frictionTable = frictionyaml.as<std::map<int, float>>();

	boost::property_tree::ptree pt;
	boost::property_tree::read_json(jsonfilename, pt);

	// taile en nombre de tuiles
	lenght = sf::Vector2u(pt.get<uint>("width"), pt.get<uint>("height"));

	std::vector<std::vector<int>> tilemapdata;
	int tilelayer = 0;

	//Parse layers
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

	//ajout dans la map pour une recherche rapide (log n)
	for (auto it = begin(tilesetdefs); it != end(tilesetdefs); ++it)
	{
		auto nextit = it + 1;
		int min = it->firstgid;
		int max;
		if (nextit == end(tilesetdefs))
		{
			max = INT_MAX;
		}
		else
		{
			max = nextit->firstgid - 1;
		}
		tiledefmap[make_range(min, max)] = it._Ptr;
	}




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
			for (auto& tilemap : tilemapdata)
			{
				int data = tilemap[k];
				if (data == 0)
				{
					continue;
				}
				const TileSetDef* currDef = tiledefmap[make_range(data, data)];

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
				auto coeff = frictionTable[data];
				coeff = coeff ? coeff : 1.f; //default value
				groundmatrix(j, i).push_back(new Ground(world, position, currDef->tilewidth, currDef->tileheight, coeff));
#endif // DISABLETERRAIN		
			}

			
			k += 1;
		}

	return true;
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
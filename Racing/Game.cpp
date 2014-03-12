#include "stdafx.h"
#include "Game.h"
#include "Car.h"

using namespace boost::filesystem;


Game::Game(const std::string& levelname) : world({ 0, 0 }), overlay(nullptr)
{
	world.SetContactListener(&contactlistener);
	
	path p("./Levels");
	if (!exists(p) || !is_directory(p))
	{
		throw std::exception("pas de repertoire Levels.");
	}
	LOG_DEBUG << "OK";
	p /= levelname;
	if (!exists(p) || !is_directory(p))
	{
		throw std::exception((std::string("pas de repertoire : Levels/") + levelname).c_str());
	}
	level.load((p/"tiled.json").generic_string(), &world);
	checkpointContactHandler = contact_listner_ptr(new CheckpointContactHandler(&level));
	contactlistener.add(checkpointContactHandler);
}


Game::~Game()
{
	if (overlay){
		delete overlay;
	}

	for (auto car : cars)
	{
		delete car;
	}
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(level, states);
	for (const auto car : cars)
	{
		target.draw(*car, states);
	}
	if (overlay){
		target.draw(*overlay, states);
	}
}


Car* Game::createCar(const std::string& name)
{
	path p("./Cars");
	if (!exists(p) || !is_directory(p))
	{
		throw std::exception("pas de repertoire Cars.");
	}
	p /= name;
	if (!exists(p) || !is_directory(p))
	{
		throw std::exception((std::string("pas de repertoire : Cars/") + name).c_str());
	}

	auto imgpath = p / "image.png";
	if (!exists(imgpath) || !is_regular_file(imgpath))
	{
		throw std::exception((std::string("pas de fichier : ") + imgpath.generic_string()).c_str());
	}
	auto yamlpath = p / "car.yml";
	if (!exists(yamlpath) || !is_regular_file(yamlpath))
	{
		throw std::exception((std::string("pas de fichier : ") + yamlpath.generic_string()).c_str());
	}
	
	Car* ret = new Car(&world, imgpath.generic_string(), yamlpath.generic_string(), level.getStartPos(cars.size()));
	cars.push_back(ret);
	return ret;
}

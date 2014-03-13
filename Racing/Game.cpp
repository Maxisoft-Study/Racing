#include "stdafx.h"
#include "Game.h"
#include "Car.h"

using namespace boost::filesystem;


Game::Game(const std::string& levelname, sf::RenderWindow& window) : world({ 0, 0 }), mainwindow(window), overlay(nullptr), groundlistener(new GroundContactHandler), pause(false)
{
	world.SetAllowSleeping(true);
	world.SetContactListener(&contactlistener);
	
	path p("./Levels");
	if (!exists(p) || !is_directory(p))
	{
		throw std::exception("pas de repertoire Levels");
	}
	LOG_DEBUG << "OK";
	p /= levelname;
	if (!exists(p) || !is_directory(p))
	{
		throw std::exception((std::string("pas de repertoire : Levels/") + levelname).c_str());
	}
	level.load((p/"tiled.json").generic_string(), &world, p);
	checkpointContactHandler = contact_listner_ptr(new CheckpointContactHandler(this, &level));
	contactlistener.add(checkpointContactHandler);
	contactlistener.add(groundlistener);
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
	//pas besoin de suppr maincar car deja dans le vector
}




void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::View old = target.getView();
		
	const b2Vec2 position(mainCar->getBody()->GetWorldCenter());
	auto center = Utils::Box2DVectToSfVectPixel(position);
	//utilise round afin d'avoir des coordonnées sans virgule (fait un décalage de sprite sinon)
	center.x = std::round(center.x);
	center.y = std::round(center.y);
	sf::View view = target.getDefaultView();
	view.setCenter(center);
	target.setView(view);

	target.draw(level, states);
	for (const auto car : cars)
	{
		target.draw(*car, states);
	}

	if (overlay){
		//reset la vue pour dessiner les overlays
		target.setView(sf::View());
		target.draw(*overlay, states);
			
	}
	target.setView(old);
	
}

void Game::setMainCar(Car* carparam)
{
	bool found = false;
	for (auto car : cars)
	{
		car->setRoundCoordinate(false);
		if (car == carparam)
		{
			found = true;
		}
	}
	if (!found)
	{
		throw std::exception("la voiture n'a pas été trouver dans le jeux.");
	}
	if (overlay)
	{
		delete overlay;
	}

	overlay = new InGameOverlay(mainwindow, carparam);

	mainCar = carparam;
	mainCar->setRoundCoordinate(true);
}

Car* Game::createCar(const std::string& name, bool maincar)
{
	path p("./Cars");
	if (!exists(p) || !is_directory(p))
	{
		throw std::exception("pas de repertoire Cars");
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
	if (maincar)
	{
		setMainCar(ret);
	}
	return ret;
}

CarControler& Game::createCarControler(Car* car, const CarControlKeysDef& controlkeys)
{
	carcontrolers.emplace_back(car, controlkeys);
	return carcontrolers.back();
}

void Game::update(float delta)
{
	sf::Clock processingClock;
	if (!pause)
	{
		world.Step(delta, 20, 20);
		delta += processingClock.restart().asSeconds();
	}


	for (auto& carcontroler : carcontrolers)
	{
		carcontroler.parseKeys();
	}

	for (auto car : cars)
	{
		car->update(delta);
	}

	if (overlay)
	{
		overlay->update(delta);
	}
}

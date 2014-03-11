#include "stdafx.h"
#include "Game.h"
#include "Car.h"

Game::Game() : world({0,0})
{
	world.SetContactListener(&contactlistener);
}


Game::~Game()
{
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
}


Car* Game::createCar(const std::string& name, const float init_pos_x, const float init_pos_y)
{
	return nullptr;
}

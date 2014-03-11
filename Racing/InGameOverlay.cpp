#include "stdafx.h"
#include "InGameOverlay.h"


InGameOverlay::InGameOverlay(sf::RenderWindow& window, const Car* carptr) : 
car(carptr)
{
	if (!speedFont.loadFromFile("ressources/Franchise-Bold-hinted.ttf"))
	{
		throw std::exception("le fichier font \"Franchise-Bold-hinted.ttf\" n'existe pas.");
	}
	speedText.setFont(speedFont);
	speedText.setColor(sf::Color::Blue);
	speedText.setCharacterSize(20);
	speedText.setPosition(static_cast<float>(window.getSize().x) - window.getSize().x / 2.f, static_cast<float>(window.getSize().y) - 20 - 15);
}


InGameOverlay::~InGameOverlay(void)
{
}

void InGameOverlay::update(float delta)
{
	speedText.setString(std::to_string((long)car->getSpeed()));
}

void InGameOverlay::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(speedText, states);
}

#pragma once
#include "stdafx.h"
class Car;
#include "Level.h"
#include "GameContactListener.h"
#include "CheckPointContactHandler.h"
#include "InGameOverlay.h"

class Game : virtual sf::Drawable
{
	friend class sf::RenderTarget;
public:
	Game(const std::string& levelname);

	//////////////////////////////////////////////////////////////////////////
	/// ajoute un contact listener
	//////////////////////////////////////////////////////////////////////////
	inline void addContactListener(const contact_listner_ptr handler)
	{
		contactlistener.add(handler);
	}

	Car* createCar(const std::string& name);
	~Game();
private:
	b2World world;
	std::vector<Car*> cars;
	Level level;
	GameContactListener contactlistener;
	contact_listner_ptr checkpointContactHandler;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
	InGameOverlay* overlay;
};


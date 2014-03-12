#pragma once
#include "stdafx.h"
class Car;
#include "Level.h"
#include "GameContactListener.h"
#include "CheckPointContactHandler.h"
#include "InGameOverlay.h"
#include "CarControler.h"
#include "GroundContactHandler.h"

class Game : public virtual sf::Drawable, public virtual Updatable
{
	friend class sf::RenderTarget;
public:
	Game(const std::string& levelname);
	~Game();

	//////////////////////////////////////////////////////////////////////////
	/// ajoute un contact listener
	//////////////////////////////////////////////////////////////////////////
	inline void addContactListener(const contact_listner_ptr handler)
	{
		contactlistener.add(handler);
	}

	inline b2World& getWorld(void) { return world; }

	Car* createCar(const std::string& name, bool roundCoordinate=false);
	CarControler& createCarControler(Car* car, const CarControlKeysDef& controlkeys);

	virtual void update(float delta) final;



private:
	b2World world;
	std::vector<Car*> cars;

	Level level;
	std::vector<CarControler> carcontrolers;
	GameContactListener contactlistener;

	contact_listner_ptr checkpointContactHandler;
	const contact_listner_ptr groundlistener;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

	InGameOverlay* overlay;
};


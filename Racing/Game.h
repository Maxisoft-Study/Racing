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

	Car* createCar(const std::string& name, bool maincar=false);
	CarControler& createCarControler(Car* car, const CarControlKeysDef& controlkeys);

	
	
	void update(float delta);
	void setMainCar(Car* car);


private:
	b2World world;
	std::vector<Car*> cars;
	Car* mainCar;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

	bool pause;

	Level level;
	std::vector<CarControler> carcontrolers;
	GameContactListener contactlistener;

	contact_listner_ptr checkpointContactHandler;
	const contact_listner_ptr groundlistener;

	InGameOverlay* overlay;
};


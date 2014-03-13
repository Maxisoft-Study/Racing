#pragma once
#include "stdafx.h"
class Car;
#include "Level.h"
#include "GameContactListener.h"
#include "InGameOverlay.h"
#include "CarControler.h"
#include "GroundContactHandler.h"



class Game : public virtual sf::Drawable, public virtual Updatable
{
	friend class sf::RenderTarget;
public:
	Game(const std::string& levelname, sf::RenderWindow& window);
	~Game();

	//////////////////////////////////////////////////////////////////////////
	/// ajoute un contact listener
	//////////////////////////////////////////////////////////////////////////
	inline void addContactListener(const contact_listner_ptr handler)
	{
		contactlistener.add(handler);
	}

	inline b2World& getWorld(void) { return world; }

	inline void registerLap(Car* c) { auto& lap = laps[c]; lap += 1; }

	Car* createCar(const std::string& name, bool maincar = false);
	CarControler& createCarControler(Car* car, const CarControlKeysDef& controlkeys);



	void update(float delta);
	void setMainCar(Car* car);


private:
	b2World world;
	std::vector<Car*> cars;
	Car* mainCar;
	std::map<Car*, int> laps;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

	bool pause;

	Level level;
	std::vector<CarControler> carcontrolers;
	GameContactListener contactlistener;

	contact_listner_ptr checkpointContactHandler;
	const contact_listner_ptr groundlistener;

	InGameOverlay* overlay;

	sf::RenderWindow& mainwindow;
};


















class CheckpointContactHandler : public ContactListenerHandler
{

public:

	CheckpointContactHandler(Game* g, const Level* lvl) : ContactListenerHandler(), level(lvl), game(g)
	{

	}


	virtual bool CheckpointContactHandler::BeginContact(b2Contact* contact, BoxGameObject* A, BoxGameObject* B)
	{
		Wheel* wheel = nullptr;
		Checkpoint* cp = nullptr;
		//CAST
		if (A->getGType() == GameObjectTypes::CheckpointType && B->getGType() == GameObjectTypes::WheelType)
		{
			cp = dynamic_cast<Checkpoint*>(A);
			wheel = dynamic_cast<Wheel*>(B);
			if (!wheel || !cp)
			{
				LOG_ERR << "1 - Failed to cast !";
				return false;
			}
		}
		else if (B->getGType() == GameObjectTypes::CheckpointType && A->getGType() == GameObjectTypes::WheelType)
		{
			cp = dynamic_cast<Checkpoint*>(B);
			wheel = dynamic_cast<Wheel*>(A);
			if (!wheel || !cp)
			{
				LOG_ERR << "2 - Failed to cast !";
				return false;
			}
		}
		else
		{
			return false;
		}

		auto& checkpoints = savedcheckpoints[wheel->getCar()];
		if (checkpoints.empty()){
			const Checkpoint* comparecp = level->getFirstCheckpoint();
			if (comparecp == cp){ // on regarde si le checkpoint cp est bien le premier de la liste
				checkpoints.emplace(cp);
				return true;
			}
			return false;
		}

		auto endit = checkpoints.rbegin();
		const Checkpoint* comparecp = level->getNextCheckpoint(*endit);

		if (endit != checkpoints.rend() && comparecp == cp){ // on regarde si le checkpoint cp est bien le suivant de la liste
			checkpoints.emplace(cp);
			if (checkpoints.size() == level->getCheckPointCount())
			{
				checkpoints.clear();
				auto car = wheel->getCar();
				game->registerLap(car);
			}
			return true;
		}

		return false;

	}


	//////////////////////////////////////////////////////////////////////////
	/// Permet de savoir le nombre de checkpoint que la voiture a parcouru
	//////////////////////////////////////////////////////////////////////////
	uint getCheckpointCount(const Car* car_ptr) const
	{
		auto it = savedcheckpoints.find(car_ptr);
		return it == savedcheckpoints.end() ? 0 : it->second.size();
	}
private:
	//////////////////////////////////////////////////////////////////////////
	/// Sauvegarde des checkpoints par voiture
	//////////////////////////////////////////////////////////////////////////
	std::unordered_map<const Car*, std::set<Checkpoint*, CheckpointPtrComp>> savedcheckpoints;
	const Level* level;
	Game* game;
};

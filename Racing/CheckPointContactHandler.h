#pragma once
#include "stdafx.h"
#include "GameContactListener.h"
#include "Checkpoint.h"
#include "LoggerConfig.h"

class CheckpointContactHandler : public ContactListenerHandler
{

public:

	bool BeginContact(b2Contact* contact, BoxGameObject* A, BoxGameObject* B)
	{
		Wheel* wheel = nullptr;
		Checkpoint* cp = nullptr;

		//CAST
		if (cp = dynamic_cast<Checkpoint*>(A))
		{
			wheel = dynamic_cast<Wheel*>(B);
			if (!wheel)
			{
				return false;
			}
		}
		else if (cp = dynamic_cast<Checkpoint*>(B))
		{
			wheel = dynamic_cast<Wheel*>(A);
			if (!wheel)
			{
				return false;
			}
		}
		else
		{
			return false;
		}

		savedcheckpoints[wheel->getCar()].emplace(cp);

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	/// Permet de savoir le nombre de checkpoint que la voiture a parcouru
	//////////////////////////////////////////////////////////////////////////
	uint getCheckpointCount(const Car* car_ptr)
	{
		return savedcheckpoints[car_ptr].size();
	}
private:
	//////////////////////////////////////////////////////////////////////////
	/// Sauvegarde des checkpoints par voiture
	//////////////////////////////////////////////////////////////////////////
	std::unordered_map<const Car*, std::set<const Checkpoint*>> savedcheckpoints;
};
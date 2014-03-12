#pragma once
#include "stdafx.h"
#include "Car.h"
#include "Wheel.h"
#include "GameContactListener.h"
#include "Checkpoint.h"
#include "LoggerConfig.h"

class CheckpointContactHandler : public ContactListenerHandler
{

public:

	CheckpointContactHandler(const Level* lvl) : ContactListenerHandler(), level(lvl)
	{
	}

	virtual bool BeginContact(b2Contact* contact, BoxGameObject* A, BoxGameObject* B) final
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
				LOG_DEBUG << "First checkpoint OK";
				checkpoints.emplace(cp);
				return true;
			}
			return false;
		}

		auto endit = checkpoints.rbegin();
		const Checkpoint* comparecp = level->getNextCheckpoint(*endit);

		if (endit != checkpoints.rend() && comparecp == cp){ // on regarde si le checkpoint cp est bien le suivant de la liste
			LOG_DEBUG << "checkpoint OK";
			checkpoints.emplace(cp);
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
};
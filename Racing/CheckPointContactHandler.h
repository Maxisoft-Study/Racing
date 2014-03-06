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

		savedcheckpoints[wheel->getCar()].emplace(cp);

		return true;
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
	std::unordered_map<const Car*, std::set<const Checkpoint*>> savedcheckpoints;
};
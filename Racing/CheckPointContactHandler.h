#pragma once
#include "stdafx.h"
#include "GameContactListener.h"
#include "LoggerConfig.h"

class CheckpointContactHandler : public ContactListenerHandler
{
	bool BeginContact(b2Contact* contact, GameObject* A, GameObject* B)
	{
		Checkpoint* checkpoint(nullptr);
		Car* car(nullptr);

		//CAST
		if (A->getGType() == GameObject::CheckpointType)
		{
			checkpoint = dynamic_cast<Checkpoint*>(A);
			if (B->getGType() == GameObject::CarType)
			{
				car = dynamic_cast<Car*>(B);
			}
			else
			{
				return false;
			}
		}
		else if (B->getGType() == GameObject::CheckpointType)
		{
			checkpoint = dynamic_cast<Checkpoint*>(B);
			if (A->getGType() == GameObject::CarType)
			{
				car = dynamic_cast<Car*>(A);
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}

		if (!car)
		{
			LOG_ERR << "Cannot cast car";
			return false;
		}
		if (!checkpoint)
		{
			LOG_ERR << "Cannot cast checkpoint";
			return false;
		}

		LOG_INFO << "CHECKPOINT OK";




		return true;
	}
	
};
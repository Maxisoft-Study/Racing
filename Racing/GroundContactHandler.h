#pragma once
#include "stdafx.h"
#include "GameContactListener.h"
#include "Ground.h"
#include "Wheel.h"
#include "LoggerConfig.h"

class GroundContactHandler : public ContactListenerHandler
{

public:

	virtual bool BeginContact(b2Contact* contact, BoxGameObject* A, BoxGameObject* B) final
	{
		Wheel* wheel = nullptr;
		Ground* ground = nullptr;

		//CAST
		if (ground = dynamic_cast<Ground*>(A))
		{
			wheel = dynamic_cast<Wheel*>(B);
			if (!wheel)
			{
				return false;
			}
		}
		else if (ground = dynamic_cast<Ground*>(B))
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


		wheel->addGround(ground);
		return true;
	}

	virtual bool EndContact(b2Contact* contact, BoxGameObject* A, BoxGameObject* B)
	{

		Wheel* wheel = nullptr;
		Ground* ground = nullptr;

		//CAST
		if (ground = dynamic_cast<Ground*>(A))
		{
			wheel = dynamic_cast<Wheel*>(B);
			if (!wheel)
			{
				return false;
			}
		}
		else if (ground = dynamic_cast<Ground*>(B))
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

		wheel->removeGround(ground);

		return true;
	}

private:
};
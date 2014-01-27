#pragma once
#include "stdafx.h"
#include "LoggerConfig.h"


class ContactListenerHandler
{
public:
	virtual bool BeginContact(b2Contact* contact, GameObject* A, GameObject* B) { return false; }
	virtual bool EndContact(b2Contact* contact, GameObject* A, GameObject* B) { return false; };

	/*
	virtual bool PreSolve(b2Contact* contact, const b2Manifold* oldManifold) = 0;
	virtual bool PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) = 0;
	*/
protected:

private:
};

typedef std::shared_ptr<ContactListenerHandler> contact_listner_ptr;


//////////////////////////////////////////////////////////////////////////
/// Gestionnaire de contact pour box2d.
/// 
//////////////////////////////////////////////////////////////////////////

class GameContactListener : public b2ContactListener
{
public:
	GameContactListener(void) : b2ContactListener(){};
	~GameContactListener(void) {};

	virtual void BeginContact(b2Contact* contact)
	{
		GameObject* A = static_cast<GameObject*>(contact->GetFixtureA()->GetBody()->GetUserData());
		GameObject* B = static_cast<GameObject*>(contact->GetFixtureB()->GetBody()->GetUserData());
		if (!A || !B)
		{
			LOG_WARN << "Unknow Object Type";
			return;
		}
			
		for (contact_listner_ptr contactListener : handlers)
		{
			
			if (contactListener->BeginContact(contact, A, B)) // => evenement gerer
				return;
		}
		LOG_DEBUG << "Unhandled Begin Contact Event.";
	}

	virtual void EndContact(b2Contact* contact)
	{
		GameObject* A = static_cast<GameObject*>(contact->GetFixtureA()->GetBody()->GetUserData());
		GameObject* B = static_cast<GameObject*>(contact->GetFixtureB()->GetBody()->GetUserData());
		if (!A || !B)
		{
			LOG_WARN << "Unknow Object Type";
			return;
		}

		for (contact_listner_ptr contactListener : handlers)
		{
		
			if (contactListener->EndContact(contact, A, B)) // => evenement gerer
				return;
		}
		LOG_DEBUG << "Unhandled End Contact Event.";
	}

	/*
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{

	}
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{

	}
	*/


	//////////////////////////////////////////////////////////////////////////
	/// ajoute un contact listener
	//////////////////////////////////////////////////////////////////////////
	void add (const contact_listner_ptr handler)
	{
		handlers.push_back(handler);
	}

	//////////////////////////////////////////////////////////////////////////
	/// enleve un contact listener
	//////////////////////////////////////////////////////////////////////////
	void remove (const contact_listner_ptr handler)
	{
		std::vector<contact_listner_ptr>::iterator newEnd = std::remove(handlers.begin(), handlers.end(), handler);
		handlers.erase(newEnd, handlers.end());
	}
protected:
	
private:
	std::vector<contact_listner_ptr> handlers;
};
#pragma once
#include "stdafx.h"


class Car;
#include "Level.h"
#include "GameContactListener.h"



class Game : virtual sf::Drawable
{
	friend class sf::RenderTarget;
public:
	Game();

	//////////////////////////////////////////////////////////////////////////
	/// ajoute un contact listener
	//////////////////////////////////////////////////////////////////////////
	inline void addContactListener(const contact_listner_ptr handler)
	{
		contactlistener.add(handler);
	}

	Car* createCar(const std::string& name, const float init_pos_x = 0.f, const float init_pos_y = 0.f);

	~Game();
private:
	b2World world;
	std::vector<Car*> cars;
	Level level;
	GameContactListener contactlistener;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
};


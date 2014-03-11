#pragma once
#include "stdafx.h"
#include "MixedGameObject.h"
class Car;
#include "Ground.h"

class Wheel:
	public BoxGameObject
{
	friend class sf::RenderTarget;
	friend class Car;
public:
	////////////////////////////////////////////////////////////
	/// \brief Enumeration des pneux
	///
	////////////////////////////////////////////////////////////
	enum WheelType : unsigned char
	{
		FRONTLEFT, //avant gauche
		FRONTRIGHT, //avant droit
		REARLEFT, //arriere gauche
		REARRIGHT, // arriere droit
		Count // nombre de pneu (4)
	};

	Wheel(b2World* world, Car* car, const WheelType type, const float init_pos_x, const float init_pos_y, const float angle = 0.f);
	~Wheel();
	virtual void update(float delta) final;
	const WheelType wheeltype;
	const virtual GameObjectTypes getGType(void) const final;
	const Ground* searchMaxGroundFriction(void) const;
	inline const Car* getCar(void) const{ return car_ptr; }
	inline void addGround(const Ground* ground) { grounds.emplace(ground); }
	inline bool removeGround(const Ground* ground) 
	{
		if (grounds.empty()){
			return false;
		}
		auto it = grounds.find(ground);
		if (it == end(grounds))
		{
			return false;
		}
		grounds.erase(it);
		return true;
	}
protected:
	b2Vec2 getLateralVelocity(void) const;
	void killOrthogonalVelocity(void);
private:
	const Car* const car_ptr;
	std::set<const Ground*> grounds;
};


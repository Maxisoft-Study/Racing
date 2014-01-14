#pragma once
#include "stdafx.h"
#include "MixedGameObject.h"
class Car;

class Wheel:
	public MixedGameObject
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

	Wheel(b2World *world, const std::string &file, Car *car, const WheelType type, const float init_pos_x, const float init_pos_y);
	~Wheel();
	virtual void update(float delta) final;
	const WheelType wheeltype;
protected:
	b2Vec2 getLateralVelocity(void) const;
	void killOrthogonalVelocity(void);
private:
	const Car * const car_ptr;
};


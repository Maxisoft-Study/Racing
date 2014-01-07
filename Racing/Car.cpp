#include "stdafx.h"
#include "Car.h"

b2Vec2 Car::DIM = b2Vec2(1.33333333333f/2, 3.01388888889f/2);

Car::Car(b2World *world, const std::string &file, const float init_pos_x, const float init_pos_y) :
MixedGameObject(world, file, init_pos_x, init_pos_y), wheels(Wheel::Count), wheelsJoints(Wheel::Count), maxfrontwheelsangle(Utils::DegreeToRadian(30))
{
	setGType(GameObject::CarType);
	bodydef->type = b2_dynamicBody;
	bodydef->linearDamping = 0.7f;
	bodydef->angularDamping = 0.7f;
	//bodydef->position.Set(0.f, 0.f);
	bodydef->bullet = true;

	body = world->CreateBody(bodydef);
	body->SetUserData(this);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(Car::DIM.x, Car::DIM.y);

	b2FixtureDef *fixtureDef = new b2FixtureDef();
	fixtureDef->shape = &dynamicBox;
	fixtureDef->density = 1.5f;
	fixtureDef->friction = 0.5f;
	body->CreateFixture(fixtureDef);

	delete fixtureDef;

	b2Vec2 wheelsinitpos[4] = { b2Vec2(-0.7f, -0.8f), b2Vec2(0.7f, -0.8f), b2Vec2(-0.7f, 1.f), b2Vec2(0.7f, 1.f) };
	auto bodyworldcenterPos = body->GetWorldCenter();

	//crée les roue
	for (uint i = 0; i < Wheel::WheelType::Count; ++i)
	{
		wheelsinitpos[i] += bodyworldcenterPos;
		Wheel *curr = new Wheel(getWorld(), "ressources/wheel.png", this, static_cast<Wheel::WheelType> (i), wheelsinitpos[i].x, wheelsinitpos[i].y);
		wheels[i] = curr;
	}

	//crée les jointures

	//roues arrieres
	//On les fixe au vehicule
	auto tmpbody = wheels[Wheel::REARLEFT]->getBody();
	b2PrismaticJointDef *leftrearjoindef = new b2PrismaticJointDef();
	leftrearjoindef->Initialize(getBody(), tmpbody, tmpbody->GetWorldCenter(), b2Vec2(1, 0));
	leftrearjoindef->enableLimit = true; //=> la roue tourne en même temps que le vehicule
	wheelsJoints[Wheel::REARLEFT] = world->CreateJoint(leftrearjoindef);
	delete leftrearjoindef;



	tmpbody = wheels[Wheel::REARRIGHT]->getBody();
	b2PrismaticJointDef *rightrearjoindef = new b2PrismaticJointDef();
	rightrearjoindef->Initialize(getBody(), tmpbody, tmpbody->GetWorldCenter(), b2Vec2(1, 0));
	rightrearjoindef->enableLimit = true;
	wheelsJoints[Wheel::REARRIGHT] = world->CreateJoint(rightrearjoindef);
	delete rightrearjoindef;


	//roues avant
	tmpbody = wheels[Wheel::FRONTLEFT]->getBody();
	b2RevoluteJointDef* leftfrontjoindef = new b2RevoluteJointDef();
	leftfrontjoindef->Initialize(getBody(), tmpbody, tmpbody->GetWorldCenter());
	leftfrontjoindef->enableMotor = true;
	leftfrontjoindef->maxMotorTorque = 100;
	wheelsJoints[Wheel::FRONTLEFT] = world->CreateJoint(leftfrontjoindef);
	delete leftfrontjoindef;

	tmpbody = wheels[Wheel::FRONTRIGHT]->getBody();
	b2RevoluteJointDef* rightfrontjoindef = new b2RevoluteJointDef();
	rightfrontjoindef->Initialize(getBody(), tmpbody, tmpbody->GetWorldCenter());
	rightfrontjoindef->enableMotor = true;
	rightfrontjoindef->maxMotorTorque = 100;
	wheelsJoints[Wheel::FRONTRIGHT] = world->CreateJoint(rightfrontjoindef);
	delete rightfrontjoindef;
}



Car::~Car(void)
{
	for (uint i = 0; i < Wheel::Count; ++i)
	{
		Wheel *curr = wheels[i];
		delete curr;
	}
}


void Car::update(const float delta)
{
	MixedGameObject::update(delta);

	for (Wheel* wheel : wheels)
	{
		wheel->update(delta);
	}
	
	applyInertia();

	lastcontrol = {}; //reset
	printf("Vitesse : %f\n", getBody()->GetLinearVelocity().Length() * 3.6f);

}


void Car::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (Wheel* wheel : wheels)
	{
		wheel->draw(target, states);
	}
	MixedGameObject::draw(target, states);
}

void Car::applyInertia(void)
{
	getBody()->ApplyAngularImpulse(0.01f * getBody()->GetInertia() * -getBody()->GetAngularVelocity() * getBody()->GetLinearVelocity().Length(), true);
}
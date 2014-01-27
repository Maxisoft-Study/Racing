#include "stdafx.h"
#include "Car.h"

Car::Car(b2World *world, const std::string &file, const float init_pos_x, const float init_pos_y) :
MixedGameObject(world, file, init_pos_x, init_pos_y), wheels(Wheel::Count), wheelsJoints(Wheel::Count), maxfrontwheelsangle(Utils::DegreeToRadian(38)), lastspeed(-1)
{
	setGType(GameObject::CarType);
	bodydef->type = b2_dynamicBody;
	bodydef->linearDamping = 0.7f;
	bodydef->angularDamping = 0.7f;
	//bodydef->position.Set(0.f, 0.f);
	bodydef->bullet = true;

	body = world->CreateBody(bodydef);
	body->SetUserData(this);


	YAML::Node caryaml = YAML::LoadFile("ressources/car.yaml");
	const float scale = caryaml["polygonsscale"].as<float>();
	YAML::Node polygonsyaml = caryaml["polygons"];


	std::vector<b2Vec2> parsedpolygonpoints(0);

	for (YAML::Node polygon : polygonsyaml)
	{
		//remplis le vector de points
		for (YAML::Node point : polygon)
		{
			parsedpolygonpoints.emplace_back(point['x'].as<float>() * scale, point['y'].as<float>() * scale);
		}

		//crée la fixture.
		b2PolygonShape *dynamicBox = new b2PolygonShape();
		dynamicBox->Set(&(parsedpolygonpoints[0]), parsedpolygonpoints.size());

		b2FixtureDef *fixtureDef = new b2FixtureDef();
		fixtureDef->shape = dynamicBox;
		fixtureDef->density = 1.6f;
		fixtureDef->friction = 0.5f;
		fixtureDef->filter.categoryBits = BoxGameObject::CAR_MASK;


		body->CreateFixture(fixtureDef);

		delete fixtureDef;
		delete dynamicBox;

		parsedpolygonpoints.clear();
	}
		
	
	

	b2Vec2 wheelsinitpos[4] = { b2Vec2(-0.65f, -1.f), b2Vec2(0.65f, -1.f), b2Vec2(-0.65f, 1.f), b2Vec2(0.65f, 1.f) };
	auto bodyworldcenterPos = body->GetWorldCenter();
	//crée le simulateur de moteur
	engine = new CarEngine(this);

	//crée les roue
	for (uint i = 0; i < Wheel::WheelType::Count; ++i)
	{
		wheelsinitpos[i] += bodyworldcenterPos;
		Wheel *curr = new Wheel(getWorld(), this, static_cast<Wheel::WheelType> (i), wheelsinitpos[i].x, wheelsinitpos[i].y);
		wheels[i] = curr;
	}

	//crée les jointures

	//roues arrières
	//On les fixe au véhicule
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
	delete engine;
}


void Car::update(const float delta)
{
	lastspeed = getBody()->GetLinearVelocity().Length() * 3.6f;

	MixedGameObject::update(delta);

	for (Wheel* wheel : wheels)
	{
		wheel->update(delta);
	}
	
	applyInertia(delta);

	lastcontrol = {}; //reset
	
	//printf("Vitesse : %f\n", lastspeed);

}


void Car::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	MixedGameObject::draw(target, states);
}

void Car::applyInertia(const float delta)
{
	//contrôle de la rotation du véhicule par rapport a son centre d'inertie.
	getBody()->ApplyAngularImpulse(0.0017f * getBody()->GetInertia() * -getBody()->GetAngularVelocity() * getSpeed(), false);

	//applique une force simulant l'inertie pour les dérapages
	/*
	// vecteur a ajouter par rapport au centre
	b2Vec2 basepointapplication(0, 0);
	basepointapplication = Utils::RotateVect(basepointapplication, getBody()->GetAngle());

	// le point d'application de la force
	b2Vec2 pointapplication = getBody()->GetWorldCenter();
	pointapplication += basepointapplication;

	
	b2Vec2 rightNormal = getBody()->GetWorldVector(b2Vec2(1, 0));
	b2Vec2 impulse = getBody()->GetMass() * -rightNormal;

	
	//la norme de la force:
	b2Vec2 force = 0.06f * getBody()->GetAngularVelocity() * getBody()->GetInertia() * impulse;
	//getBody()->ApplyLinearImpulse(force, getBody()->GetWorldCenter(), false);

	//getBody()->ApplyLinearImpulse(force, pointapplication, false);
	//getBody()->ApplyForce(force, pointapplication, false);
	//std::cout << getSide() << std::endl;
	*/
}

const float Car::getSpeed(void) const
{
	return lastspeed;
}

const CarControl Car::getlastControl(void) const
{
	return lastcontrol;
}


const CarSide Car::getSide(void) const
{
	//dot product
	b2Vec2 v1 = getBody()->GetLinearVelocity();
	b2Vec2 v2 = getBody()->GetTransform().q.GetYAxis();

	return b2Dot(v1, v2) > 0 ? CarSide::back : CarSide::ahead;
}


float Car::getMaxFrontWheelsAngle(void) const
{
	return maxfrontwheelsangle;
}

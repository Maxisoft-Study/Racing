#include "stdafx.h"
#include "Car.h"
#include "Wheel.h"
#include "CarEngine.h"
#include "CarControler.h"
#include "TextureLoader.h"
#include "YamlConverter.h"

Car::Car(b2World* world, const std::string& file, const float init_pos_x, const float init_pos_y, const float angle) :
MixedGameObject(world, file),
wheels(Wheel::Count),
wheelsJoints(Wheel::Count),
maxfrontwheelsangle(Utils::DegreeToRadian(38)), lastspeed(-1)
{
	b2BodyDef bodydef;
	bodydef.type = b2_dynamicBody;
	bodydef.linearDamping = 0.7f;
	bodydef.angularDamping = 0.7f;
	bodydef.position.Set(init_pos_x, init_pos_y);
	//bodydef.angle = Utils::DegreeToRadian(angle);
	bodydef.bullet = true;
	
	body = world->CreateBody(&bodydef);
	body->SetUserData(this);

	YAML::Node caryaml = YAML::LoadFile("ressources/car.yaml");
	auto spritesize = TextureLoader::Instance().get(file)->getSize();
	const float scale = spritesize.x / racing::BOX2D_METERS_TO_PIXEL;

	std::vector<b2Vec2> parsedpolygonpoints;

	for (YAML::Node polygon : caryaml["polygons"])
	{
		//remplis le vector de points
		for (YAML::Node point : polygon)
		{
			parsedpolygonpoints.emplace_back(scale * point.as<b2Vec2>());
		}

		//crée la fixture.
		b2PolygonShape* dynamicBox = new b2PolygonShape();
		dynamicBox->Set(&(parsedpolygonpoints[0]), parsedpolygonpoints.size());

		b2FixtureDef fixtureDef;
		fixtureDef.shape = dynamicBox;
		fixtureDef.density = 2.68f;
		fixtureDef.friction = 0.5f;
		fixtureDef.filter.categoryBits = BoxGameObject::CAR_MASK;
		//fixtureDef->filter.maskBits |= BoxGameObject::CHECKPOINT_MASK;


		body->CreateFixture(&fixtureDef);

		delete dynamicBox;

		parsedpolygonpoints.clear();
	}
		
	auto bodyworldcenterPos = body->GetWorldCenter();

	//crée le simulateur de moteur
	engine = new CarEngine(this);


	YAML::Node wheelsyml = caryaml["wheels"];
	//crée les roue
	for (uint i = 0; i < Wheel::WheelType::Count; ++i)
	{
		b2Vec2 weelposition = wheelsyml[i].as<b2Vec2>();
		//LOG_DEBUG << "x :" << weelposition.y;
		//weelposition = Utils::RotateVect(weelposition, PI);
		//LOG_DEBUG << "x :" << weelposition.y;
		weelposition += bodyworldcenterPos;
		Wheel *curr = new Wheel(getWorld(), this, static_cast<Wheel::WheelType> (i), weelposition.x, weelposition.y);
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



	//on met a jour les angles manuellement
	auto tmpangle = Utils::DegreeToRadian(angle);
	getBody()->SetTransform(getBody()->GetPosition(), tmpangle);
	for (uint i = 0; i < Wheel::WheelType::Count; ++i)
	{
		Wheel* curr = wheels[i];
		curr->getBody()->SetTransform(curr->getBody()->GetPosition(), tmpangle);
	}

}



Car::~Car(void)
{
	for (uint i = 0; i < Wheel::Count; ++i)
	{
		Wheel* curr = wheels[i];
		delete curr;
	}
	delete engine;
}


void Car::update(const float delta)
{
	MixedGameObject::update(delta);

	lastspeed = getBody()->GetLinearVelocity().Length() * 3.6f;

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
	getBody()->ApplyAngularImpulse(0.08f * delta * getBody()->GetInertia() * -getBody()->GetAngularVelocity() * getSpeed(), false);

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

	return b2Dot(v1, v2) > 0 ? CarSide::Back : CarSide::Ahead;
}


float Car::getMaxFrontWheelsAngle(void) const
{
	return maxfrontwheelsangle;
}

const GameObjectTypes Car::getGType(void) const 
{
	return GameObjectTypes::CarType;
}

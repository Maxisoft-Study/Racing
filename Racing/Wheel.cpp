#include "stdafx.h"
#include "Wheel.h"


Wheel::Wheel(b2World *world, const std::string &file, Car* car, const WheelType type, const float init_pos_x, const float init_pos_y) :
MixedGameObject(world, file, init_pos_x, init_pos_y),
car_ptr(car), 
wheeltype(type)
{
	setGType(GameObject::WheelType);
	bodydef->type = b2_dynamicBody;
	body = world->CreateBody(bodydef);
	body->SetUserData(this);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(0.1f, 0.3f);

	b2FixtureDef *fixtureDef = new b2FixtureDef();
	fixtureDef->shape = &dynamicBox;
	fixtureDef->density = 1.f;
	fixtureDef->friction = 0.5f;

	//Permet de ne plus avoir de collision avec les roues
	fixtureDef->filter.maskBits = 0x0;
	fixtureDef->filter.groupIndex = -8;

	body->CreateFixture(fixtureDef);

	delete fixtureDef;
}


Wheel::~Wheel()
{
}

void Wheel::update(float delta)
{
	killOrthogonalVelocity();
	if (wheeltype == Wheel::FRONTLEFT || wheeltype == Wheel::FRONTRIGHT) // les pneux de devant
	{
		float carangle = car_ptr->getBody()->GetAngle();

		if (car_ptr->lastcontrol.rotation) // l'utilisateur veut tourner.
		{
			float angle = car_ptr->lastcontrol.rotation * 1.6f * delta + getBody()->GetAngle();

			if (std::abs(angle - carangle) <= car_ptr->maxfrontwheelsangle) // verification de l'angle max que peut avoir un pneu par rapport a la voiture
			{
				getBody()->SetTransform(getBody()->GetPosition(), angle);
			}
		}

		//autocentrage des pneux
		float angle = getBody()->GetAngle();
		const float diffangle = angle - carangle;
		b2RevoluteJoint *joint = static_cast<b2RevoluteJoint *> (car_ptr->wheelsJoints[wheeltype]);
		if (diffangle > 0.01f)
		{
			joint->SetMotorSpeed(-1.2f);
		}
		else if (diffangle < -0.01f)
		{
			joint->SetMotorSpeed(+1.2f);
		}
		else if (getBody()->IsAwake())
		{
			joint->SetMotorSpeed(0.f);
		}

		if (car_ptr->lastcontrol.direction) //l'utilisateur veut faire fonctionner le moteur
		{
			b2Vec2 force(0.f, 50.f * delta * car_ptr->lastcontrol.direction);
			force = Utils::RotateVect(force, getBody()->GetAngle());
			getBody()->ApplyLinearImpulse(force, getBody()->GetWorldCenter(), true);
		}

	}

	MixedGameObject::update(delta);
}

void Wheel::killOrthogonalVelocity(void)
{
	b2Vec2 velocity = getBody()->GetLinearVelocityFromLocalPoint(b2Vec2(0, 0));
	auto sidewayaxis = getBody()->GetTransform().q.GetYAxis();
	sidewayaxis *= b2Dot(velocity, sidewayaxis);
	getBody()->SetLinearVelocity(sidewayaxis);
	getBody()->ApplyAngularImpulse(0.1f * getBody()->GetInertia() * -getBody()->GetAngularVelocity(), true);
}
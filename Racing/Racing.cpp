#include "stdafx.h"
#include "GConst.h"
#include "EventHandler.h"
#include "BaseEventHandler.h"
#include "Car.h"

using std::cout;
using std::vector;


//TODO
class CarControler
{
public:
	CarControler(Car *car): car_ptr(car){}
	virtual bool parseKeys()
	{
		sf::Vector2f move;
		float rot = 0.f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			car_ptr->getBody()->ApplyLinearImpulse(b2Vec2(-200.0f, 0.0f), car_ptr->getBody()->GetWorldCenter(), true);
			//move.x -= 1;
			rot -= 1;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			auto angle = car_ptr->getBody()->GetAngle();
			b2Vec2 vect = b2Vec2(0.0f, -50.0f);
			vect.x *= -std::sin(angle);
			vect.y *= std::cos(angle);

			car_ptr->getBody()->ApplyLinearImpulse(b2Vec2(0.f, -50.0f), car_ptr->getBody()->GetWorldCenter(), true);
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			auto point_apply_force = car_ptr->getBody()->GetWorldCenter();
			auto angle = car_ptr->getBody()->GetAngle();
			point_apply_force.y += 1.2f;
			point_apply_force = Utils::rotatePoint(point_apply_force, angle);

			auto vect = Utils::rotateVect(b2Vec2(30.f, 0.f), angle);


			car_ptr->getBody()->ApplyLinearImpulse(vect, point_apply_force, true);
			//move.x += 1;
			rot += 1;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			car_ptr->getBody()->ApplyLinearImpulse(b2Vec2(0.0f, 50.0f), car_ptr->getBody()->GetWorldCenter(), true);
			move.y += 2;
		}
		move.x *= 3;
		move.y *= 3;

		//car_ptr->move(move);
		//car_ptr->rotate(rot);
		return move.x || move.y || rot;
	}
protected:
private:
	Car *car_ptr;
};


int main()
{
	vector<vector<int> > map(50);
	for(int i = 0; i< 50; ++i){
		for(int j = 0; j < 50; ++j){
			map[i].push_back(0);
		}
	}
	SpriteGameObject sprite_herbe("texture_herbe.png");
	SpriteGameObject sprite_route("texture_route.png");






	sf::Clock clock;

	b2Vec2 gravity(0.0f, 0.0f);
	b2World world(gravity);

	racing::init();
	Car testcar(&world, "voituretest.png");
	//testcar.rotate(90);
	//testcar.move(sf::Vector2f(1280/2, 600));

	CarControler ccontroler(&testcar);

	sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!", sf::Style::Default, racing::settings);

	sf::View view(window.getDefaultView());
	view.zoom(2); // zoom out
	window.setView(view);

	window.setVerticalSyncEnabled(true);
	racing::EVENTS_HANDLERS.push_back(new BaseEventHandler(&window));
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			for(auto handler : racing::EVENTS_HANDLERS) // TEST C++11
			{
				if (handler->on_event(event))
				{
					break;
				}
			}
		}

		sf::Time elapsed = clock.restart();

		world.Step(elapsed.asSeconds(), 20, 20);
		world.ClearForces();



		b2Vec2 position = testcar.getBody()->GetWorldCenter();
		printf("%4.2f %4.2f\n\n", position.x, position.y);


		ccontroler.parseKeys();

		window.clear();
		testcar.update(elapsed.asSeconds());
		view.setCenter(position.x * 72, position.y * 72);

		window.setView(view);
		for(int i = 0; i< 50; ++i){
			for(int j = 0; j < 50; ++j){
				sprite_herbe.setPosition(sf::Vector2f(i*200.f, j*200.f));
				if(map[i][j] == 0)
				{
					window.draw(sprite_herbe);
				}
			}
		}


		window.draw(testcar);

		//Center


		window.display();
	}

	//return 0;
}

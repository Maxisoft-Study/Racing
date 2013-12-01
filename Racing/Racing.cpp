#include "stdafx.h"
#include "GConst.h"
#include "EventHandler.h"
#include "BaseEventHandler.h"
#include "Car.h"

using std::cout;

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
			//move.x -= 1;
			rot -= 1;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			move.y -= 2;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			//move.x += 1;
			rot += 1;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
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

	sf::Clock clock;

	b2Vec2 gravity(0.0f, 0.0f);
	b2World world(gravity);

	racing::init();
	Car testcar(&world, "voituretest.png");
	testcar.rotate(90);
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
		cout << printf("%4.2f %4.2f\n\n", position.x, position.y);
		testcar.getBody()->ApplyLinearImpulse(b2Vec2(0.0f, 50.0f), testcar.getBody()->GetWorldCenter(), true);

		ccontroler.parseKeys();

        window.clear();
		window.draw(testcar);
		
		//Center
		view.setCenter(position.x, position.y);
		window.setView(view);
		
        window.display();
    }

    return 0;
}
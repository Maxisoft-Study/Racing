#include "stdafx.h"
#include "GConst.h"
#include "EventHandler.h"
#include "BaseEventHandler.h"
#include "Car.h"

using std::cout;


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

		car_ptr->move(move);
		car_ptr->rotate(rot);
		return move.x || move.y || rot;
	}
protected:
private:
	Car *car_ptr;
};


int main()
{
	int rot = 0;
	racing::init();
	Car testcar( sf::Vector2u(500,250) ,"voituretest.png");
	testcar.rotate(90);
	testcar.move(sf::Vector2f(1280/2, 600));

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
			for(auto handler : racing::EVENTS_HANDLERS)
			{
				if (handler->on_event(event))
				{
					break;
				}
			}
        }


		ccontroler.parseKeys();

        window.clear();
		window.draw(testcar);
		auto vect = testcar.getPositionInt();
		/*
		//Center
		view.setCenter(testcar.getPosition());
		window.setView(view);
		*/
		cout << "(" << vect.x << ", "<< vect.y << ")" << std::endl;
        window.display();
    }

    return 0;
}
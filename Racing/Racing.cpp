#include "stdafx.h"
#include "GConst.h"
#include "EventHandler.h"
#include "BaseEventHandler.h"
#include "Car.h"
#include "TextureLoader.h"
#include "Level.h"
#include "GameContactListener.h"
#include "CheckPointContactHandler.h"

#define _DEBUG_DRAW 
#ifdef _DEBUG_DRAW
#include "SFMLDebugDraw.h"

//Class copied from http://code.google.com/p/box2d/source/browse/trunk/Box2D/Testbed/Framework/Test.cpp
//Copyright (c) 2011 Erin Catto http://box2d.org
class QueryCallback : public b2QueryCallback
{
public:
	QueryCallback(const b2Vec2& point)
	{
		m_point = point;
		m_fixture = NULL;
	}

	bool ReportFixture(b2Fixture* fixture)
	{
		b2Body* body = fixture->GetBody();
		if (body->GetType() == b2_dynamicBody)
		{
			bool inside = fixture->TestPoint(m_point);
			if (inside)
			{
				m_fixture = fixture;

				// We are done, terminate the query.
				return false;
			}
		}

		// Continue the query.
		return true;
	}

	b2Vec2 m_point;
	b2Fixture* m_fixture;
};


//Converts SFML's vector to Box2D's vector and downscales it so it fits Box2D's MKS units
template<typename T >
b2Vec2 sfVecToB2Vec(sf::Vector2<T> vector)
{
	return b2Vec2(vector.x / sfdd::SCALE, vector.y / sfdd::SCALE);
}

//Creates a box body and returns a reference to it
b2Body* createSquare(b2World &world, sf::RenderWindow &window)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = sfVecToB2Vec(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
	b2Body* body = world.CreateBody(&bodyDef);


	b2PolygonShape boxShape;
	boxShape.SetAsBox(0.5f, 0.5f, b2Vec2(0.f, 0.f), 0);
	body->CreateFixture(&boxShape, 1.f);

	return body;
}

//Creates a circle body and returns a reference to it
b2Body* createCircle(b2World &world, sf::RenderWindow &window)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = sfVecToB2Vec(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
	b2Body* body = world.CreateBody(&bodyDef);


	b2CircleShape circleShape;
	circleShape.m_radius = 0.5f;
	circleShape.m_p.SetZero();
	body->CreateFixture(&circleShape, 1.f);

	return body;
}

#endif // _DEBUG_DRAW
using std::cout;
using std::vector;


//TODO
class CarControler
{
public:
	CarControler(Car *car): car_ptr(car){}
	virtual bool parseKeys()
	{
		float dir = 0.f;
		float rot = 0.f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			rot -= 1.f;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			dir -= 1.f;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			rot += 1.f;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			dir += 1.f;
		}

		car_ptr->lastcontrol = { dir, rot };

		return dir || rot;
	}
protected:
private:
	Car *car_ptr;
};


//////////////////////////////////////////////////////////////////////////
/// Register and set WORKING_DIRECTORY according to argv parameters.
//////////////////////////////////////////////////////////////////////////
void set_working_dir(char** argv)
{
	boost::filesystem::path full_path(boost::filesystem::initial_path<boost::filesystem::path>());
	full_path = boost::filesystem::system_complete(boost::filesystem::path(argv[0]));

	boost::filesystem::current_path(full_path.remove_filename());
}


std::list<EventHandler*> EVENTS_HANDLERS;
TextureLoader racing::TEXTURE_LOADER = TextureLoader();


int main(int argc, char** argv)
{
	set_working_dir(argv);

	sf::Clock clock;
	sf::Clock processingClock;

	b2Vec2 gravity(0.0f, 0.0f);
	b2World *world = new b2World(gravity);
	world->SetAllowSleeping(true);

#ifdef _DEBUG_DRAW
	/* Create window */
	sf::RenderWindow debugwindow(sf::VideoMode(1024, 768, 32), "Box2D - SFML Debug Draw Test");
	float debugviewzoom = 1;
	//debugwindow.setVerticalSyncEnabled(true);

	/* Initialize font */
	sf::Font mainFont;
	if (!mainFont.loadFromFile("ressources\\Franchise-Bold-hinted.ttf")) // Set path to your font
	{
		return 1;
	}
	/* Initialize debug text */
	sf::Text fpsCounter;
	fpsCounter.setFont(mainFont);
	fpsCounter.setColor(sf::Color::White);

	sf::Text helpText;
	helpText.setFont(mainFont);
	helpText.setColor(sf::Color::White);
	helpText.setCharacterSize(20);
	helpText.setPosition(static_cast<float>(debugwindow.getSize().x) - static_cast<float>(debugwindow.getSize().x / 2.f), 0.f);

	std::string helpTextString = std::string("Press 1 to spawn a box\n") +
		std::string("Press 2 to spawn a ball\n") +
		std::string("Press backspace to remove the most recent body\n") +
		std::string("Press F1 to enable/disable drawing of shapes\n") +
		std::string("Press F2 to enable/disable drawing of AABB's\n") +
		std::string("Press F3 to enable/disable drawing of center of gravity for objects\n") +
		std::string("Press Tab to show/hide this text");

	helpText.setString(helpTextString);

	/* Create a stringstream for conversion purposes */
	std::stringstream sstream;
	/* Initialize SFML Debug Draw */
	SFMLDebugDraw debugDraw(debugwindow);
	world->SetDebugDraw(&debugDraw);

	/* Set initial flags for what to draw */
	debugDraw.SetFlags(b2Draw::e_shapeBit); //Only draw shapes

	/* Mouse Joint */
	b2MouseJoint* mouseJoint = nullptr;
	b2BodyDef bodyDef;
	b2Body* ground = world->CreateBody(&bodyDef); //This is not the body of the bounding box
	//This body exists to serve as an anchor point for the mouse joint
	bool helpTextEnabled = true;
	std::vector<b2Body*> bodies;
	sf::Clock deltaClock; //Clock used to measure FPS
#endif // _DEBUG_DRAW

	
	//testcar.rotate(90);
	//testcar.move(sf::Vector2f(1280/2, 600));

	sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!", sf::Style::Default, racing::settings);
	sf::View view(window.getDefaultView());
	view.zoom(3.2f); // zoom out
	window.setView(view);

	window.setVerticalSyncEnabled(true);
	EVENTS_HANDLERS.push_back(new BaseEventHandler(&window));

	const int rawlevel[] =
	{
		0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 19, 19, 19,
		19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 20, 20, 20,
		20, 20, 0, 0, 0, 0, 19, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
		20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 0, 0, 0, 20, 20, 20, 20, 20, 20,
		20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};
	std::vector<uint> level(0);
	level.assign(rawlevel, rawlevel + (30 * 30));

	uint spacing = 41;
	std::string imagepath("ressources/simpleroads.png");
	uint height = 30;
	uint width = 30;
	uint tilewidth = 678;
	uint tileheight = 678;


	// on crée la tilemap avec le niveau précédemment défini

	TileMap tmap;
	if (!tmap.load(imagepath, sf::Vector2u(tilewidth, tileheight), level, width, height, 1, 20, 43))
		return -1;
	Car testcar(world, "ressources/voituretest.png");
	CarControler ccontroler(&testcar);
	Car testcar2(world, "ressources/voituretest.png", 5.f, 5.f);


	Level *lvl = new Level();
	lvl->load("ressources/test3.json", world);
	
	GameContactListener *contactlistener = new GameContactListener();
	
	std::shared_ptr<CheckpointContactHandler> checkpointlistener(new CheckpointContactHandler());

	contactlistener->add(checkpointlistener);
	world->SetContactListener(contactlistener);


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			for(auto handler : EVENTS_HANDLERS) // TEST C++11
			{
				if (handler->on_event(event))
				{
					break;
				}
			}
		}

#ifdef _DEBUG_DRAW
		sf::Time deltaTime = deltaClock.restart();
		while (debugwindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				debugwindow.close();
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F1)
			{
				if (debugDraw.GetFlags() & b2Draw::e_shapeBit) debugDraw.ClearFlags(b2Draw::e_shapeBit);
				else debugDraw.AppendFlags(b2Draw::e_shapeBit);
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F2)
			{
				if (debugDraw.GetFlags() & b2Draw::e_aabbBit) debugDraw.ClearFlags(b2Draw::e_aabbBit);
				else debugDraw.AppendFlags(b2Draw::e_aabbBit);
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F3)
			{
				if (debugDraw.GetFlags() & b2Draw::e_centerOfMassBit) debugDraw.ClearFlags(b2Draw::e_centerOfMassBit);
				else debugDraw.AppendFlags(b2Draw::e_centerOfMassBit);
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab)
			{
				if (helpTextEnabled) helpTextEnabled = false;
				else helpTextEnabled = true;
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num1)
			{
				bodies.push_back(createSquare(*world, debugwindow));
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num2)
			{
				bodies.push_back(createCircle(*world, debugwindow));
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Subtract)
			{
				debugviewzoom += 0.5;
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Add)
			{
				debugviewzoom -= 0.5;
			}
			else if (event.type == sf::Event::MouseWheelMoved)
			{
				debugviewzoom -= event.mouseWheel.delta * 0.15f;
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::BackSpace)
			{
				if (!bodies.empty())
				{
					world->DestroyBody(bodies.back());
					bodies.pop_back();
				}
			}
			// Following three events are copied almost completely from http://code.google.com/p/box2d/source/browse/trunk/Box2D/Testbed/Framework/Test.cpp
			// Copyright (c) 2011 Erin Catto http://box2d.org
			else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && mouseJoint == nullptr)
			{
				b2Vec2 mousePos = sfVecToB2Vec(debugwindow.mapPixelToCoords(sf::Mouse::getPosition(debugwindow)));
				//mousePos *= debugviewzoom;
				printf("%f, %f \n", mousePos.x, mousePos.y);

				// Make a small box.
				b2AABB aabb;
				b2Vec2 d;
				d.Set(0.001f, 0.001f);
				aabb.lowerBound = mousePos + d;
				aabb.upperBound = mousePos - d;

				// Query the world for overlapping shapes.
				QueryCallback callback(mousePos);
				world->QueryAABB(&callback, aabb);

				if (callback.m_fixture)
				{
					b2Body* body = callback.m_fixture->GetBody();
					b2MouseJointDef md;
					md.bodyA = ground; //If bounding box body is used instead, the dynamic bodes can be dragged over the bounding box and we don't want that
					md.bodyB = body;
					md.target = mousePos;
					md.maxForce = 1000.0f * body->GetMass();
					mouseJoint = (b2MouseJoint*)world->CreateJoint(&md);
					body->SetAwake(true);
				}
			}
			else if (event.type == sf::Event::MouseMoved && mouseJoint != nullptr)
			{
				b2Vec2 mousePos = sfVecToB2Vec(debugwindow.mapPixelToCoords(sf::Mouse::getPosition(debugwindow)));
				mouseJoint->SetTarget(mousePos);
			}
			else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && mouseJoint != nullptr)
			{
				world->DestroyJoint(mouseJoint);
				mouseJoint = nullptr;
			}
		}
		debugwindow.clear();
		auto contentview = debugwindow.getDefaultView();
		debugwindow.setView(sf::View());
		//Display FPS
		sstream.precision(0);
		sstream << std::fixed << "FPS: " << 1.f / deltaTime.asSeconds();
		fpsCounter.setString(sstream.str());
		debugwindow.draw(fpsCounter);
		sstream.str("");
		

		if (helpTextEnabled)
		{
			debugwindow.draw(helpText);
		}
		auto tmp = testcar.getBody()->GetWorldCenter();
		contentview.setCenter(sf::Vector2f(tmp.x * sfdd::SCALE, tmp.y * sfdd::SCALE));
		contentview.zoom(debugviewzoom);
		debugwindow.setView(contentview);
		world->DrawDebugData();

		debugwindow.display();
#endif // _DEBUG_DRAW



		processingClock.restart();
		sf::Time elapsed = clock.restart();
		float elapsedassecond = elapsed.asSeconds();
		world->Step(elapsedassecond, 20, 20);
		//world.ClearForces();


		const b2Vec2 position = testcar.getBody()->GetWorldCenter();

		ccontroler.parseKeys();

		elapsedassecond += processingClock.restart().asSeconds();
		testcar.update(elapsedassecond);
		testcar2.update(elapsedassecond);
		window.clear();
		//Center
		view.setCenter(Utils::Box2DVectToSfVectPixel(position));
		window.setView(view);

		window.draw(tmap);


		window.draw(testcar);
		window.draw(testcar2);
		//LOG_DEBUG << checkpointlistener->getCheckpointCount(&testcar);
		


		window.display();
	}

	

	return 0;
}
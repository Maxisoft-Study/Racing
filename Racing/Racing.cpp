#include "stdafx.h"
#include "GConst.h"
#include "EventHandler.h"
#include "BaseEventHandler.h"
#include "Car.h"
#include "CarControler.h"
#include "TextureLoader.h"
#include "Level.h"
#include "GameContactListener.h"
#include "CheckPointContactHandler.h"
#include "GroundContactHandler.h"

//#define _DEBUG_DRAW 



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

//////////////////////////////////////////////////////////////////////////
/// GLOABALS
std::list<EventHandler*> EVENTS_HANDLERS;
TextureLoader racing::TEXTURE_LOADER;
YAML::Node racing::CONFIG;
sf::VideoMode racing::VIDEO_MODE;
boost::bimap<const std::string, const sf::Keyboard::Key> racing::STRING_TO_SFML_KEYBOARD_KEY;
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
/// Register and set working directory according to argv parameters.
//////////////////////////////////////////////////////////////////////////
void set_working_dir(char** argv)
{
	boost::filesystem::path full_path(boost::filesystem::initial_path<boost::filesystem::path>());
	full_path = boost::filesystem::system_complete(boost::filesystem::path(argv[0]));

	boost::filesystem::current_path(full_path.remove_filename());
}

void setup_globals(void)
{
	racing::CONFIG = YAML::LoadFile("ressources/config.yml");
	racing::VIDEO_MODE = sf::VideoMode(racing::CONFIG["window"]["modeWidth"].as<uint>(), racing::CONFIG["window"]["modeHeight"].as<uint>());

	const std::map<const std::string, const sf::Keyboard::Key> string_to_sfml_keyboard_key = {
		{ "Unknown", sf::Keyboard::Unknown },
		{ "A", sf::Keyboard::A },
		{ "B", sf::Keyboard::B },
		{ "C", sf::Keyboard::C },
		{ "D", sf::Keyboard::D },
		{ "E", sf::Keyboard::E },
		{ "F", sf::Keyboard::F },
		{ "G", sf::Keyboard::G },
		{ "H", sf::Keyboard::H },
		{ "I", sf::Keyboard::I },
		{ "J", sf::Keyboard::J },
		{ "K", sf::Keyboard::K },
		{ "L", sf::Keyboard::L },
		{ "M", sf::Keyboard::M },
		{ "N", sf::Keyboard::N },
		{ "O", sf::Keyboard::O },
		{ "P", sf::Keyboard::P },
		{ "Q", sf::Keyboard::Q },
		{ "R", sf::Keyboard::R },
		{ "S", sf::Keyboard::S },
		{ "T", sf::Keyboard::T },
		{ "U", sf::Keyboard::U },
		{ "V", sf::Keyboard::V },
		{ "W", sf::Keyboard::W },
		{ "X", sf::Keyboard::X },
		{ "Y", sf::Keyboard::Y },
		{ "Z", sf::Keyboard::Z },
		{ "Num0", sf::Keyboard::Num0 },
		{ "Num1", sf::Keyboard::Num1 },
		{ "Num2", sf::Keyboard::Num2 },
		{ "Num3", sf::Keyboard::Num3 },
		{ "Num4", sf::Keyboard::Num4 },
		{ "Num5", sf::Keyboard::Num5 },
		{ "Num6", sf::Keyboard::Num6 },
		{ "Num7", sf::Keyboard::Num7 },
		{ "Num8", sf::Keyboard::Num8 },
		{ "Num9", sf::Keyboard::Num9 },
		{ "Escape", sf::Keyboard::Escape },
		{ "LControl", sf::Keyboard::LControl },
		{ "LShift", sf::Keyboard::LShift },
		{ "LAlt", sf::Keyboard::LAlt },
		{ "LSystem", sf::Keyboard::LSystem },
		{ "RControl", sf::Keyboard::RControl },
		{ "RShift", sf::Keyboard::RShift },
		{ "RAlt", sf::Keyboard::RAlt },
		{ "RSystem", sf::Keyboard::RSystem },
		{ "Menu", sf::Keyboard::Menu },
		{ "LBracket", sf::Keyboard::LBracket },
		{ "RBracket", sf::Keyboard::RBracket },
		{ "SemiColon", sf::Keyboard::SemiColon },
		{ "Comma", sf::Keyboard::Comma },
		{ "Period", sf::Keyboard::Period },
		{ "Quote", sf::Keyboard::Quote },
		{ "Slash", sf::Keyboard::Slash },
		{ "BackSlash", sf::Keyboard::BackSlash },
		{ "Tilde", sf::Keyboard::Tilde },
		{ "Equal", sf::Keyboard::Equal },
		{ "Dash", sf::Keyboard::Dash },
		{ "Space", sf::Keyboard::Space },
		{ "Return", sf::Keyboard::Return },
		{ "BackSpace", sf::Keyboard::BackSpace },
		{ "Tab", sf::Keyboard::Tab },
		{ "PageUp", sf::Keyboard::PageUp },
		{ "PageDown", sf::Keyboard::PageDown },
		{ "End", sf::Keyboard::End },
		{ "Home", sf::Keyboard::Home },
		{ "Insert", sf::Keyboard::Insert },
		{ "Delete", sf::Keyboard::Delete },
		{ "Add", sf::Keyboard::Add },
		{ "Subtract", sf::Keyboard::Subtract },
		{ "Multiply", sf::Keyboard::Multiply },
		{ "Divide", sf::Keyboard::Divide },
		{ "Left", sf::Keyboard::Left },
		{ "Right", sf::Keyboard::Right },
		{ "Up", sf::Keyboard::Up },
		{ "Down", sf::Keyboard::Down },
		{ "Numpad0", sf::Keyboard::Numpad0 },
		{ "Numpad1", sf::Keyboard::Numpad1 },
		{ "Numpad2", sf::Keyboard::Numpad2 },
		{ "Numpad3", sf::Keyboard::Numpad3 },
		{ "Numpad4", sf::Keyboard::Numpad4 },
		{ "Numpad5", sf::Keyboard::Numpad5 },
		{ "Numpad6", sf::Keyboard::Numpad6 },
		{ "Numpad7", sf::Keyboard::Numpad7 },
		{ "Numpad8", sf::Keyboard::Numpad8 },
		{ "Numpad9", sf::Keyboard::Numpad9 },
		{ "F1", sf::Keyboard::F1 },
		{ "F2", sf::Keyboard::F2 },
		{ "F3", sf::Keyboard::F3 },
		{ "F4", sf::Keyboard::F4 },
		{ "F5", sf::Keyboard::F5 },
		{ "F6", sf::Keyboard::F6 },
		{ "F7", sf::Keyboard::F7 },
		{ "F8", sf::Keyboard::F8 },
		{ "F9", sf::Keyboard::F9 },
		{ "F10", sf::Keyboard::F10 },
		{ "F11", sf::Keyboard::F11 },
		{ "F12", sf::Keyboard::F12 },
		{ "F13", sf::Keyboard::F13 },
		{ "F14", sf::Keyboard::F14 },
		{ "F15", sf::Keyboard::F15 },
		{ "Pause", sf::Keyboard::Pause },
		{ "KeyCount", sf::Keyboard::KeyCount } };

		//charge le tout dans la bimap
		racing::STRING_TO_SFML_KEYBOARD_KEY.left.insert(string_to_sfml_keyboard_key.begin(), string_to_sfml_keyboard_key.end());
}


int main(int argc, char** argv)
{
	set_working_dir(argv);
	setup_globals();

	sf::Clock clock;
	sf::Clock processingClock;

	b2Vec2 gravity(0.0f, 0.0f); // désactive la gravité
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
	
	sf::RenderWindow window(racing::VIDEO_MODE, "SFML works!", sf::Style::Default, racing::settings);
	sf::View view(window.getDefaultView());
	view.zoom(4.f); // zoom out
	window.setView(view);

	window.setVerticalSyncEnabled(true);
	//window.setFramerateLimit(30);
	EVENTS_HANDLERS.push_back(new BaseEventHandler(&window));


	//construction voitures
	Car testcar(world, "ressources/voituretest.png");
	Car testcar2(world, "ressources/voituretest.png", 5.f, 5.f);

	const CarControlKeysDef controls(racing::CONFIG["controls"].as<CarControlKeysDef>());
	CarControler carcontroler(&testcar, controls);


	Level* lvl = new Level();
	lvl->load("ressources/test3.json", world);
	
	GameContactListener* contactlistener = new GameContactListener();
	world->SetContactListener(contactlistener);

	std::shared_ptr<CheckpointContactHandler> checkpointlistener(new CheckpointContactHandler());
	contactlistener->add(checkpointlistener);
	
	std::shared_ptr<GroundContactHandler> groundlistener(new GroundContactHandler);
	contactlistener->add(groundlistener);


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
		sf::Time elapsed(clock.restart());
		float elapsedassecond = elapsed.asSeconds();
		world->Step(elapsedassecond, 20, 20);
		//world.ClearForces();


		const b2Vec2 position(testcar.getBody()->GetWorldCenter());

		carcontroler.parseKeys();

		elapsedassecond += processingClock.restart().asSeconds();
		testcar.update(elapsedassecond);
		testcar2.update(elapsedassecond);
		window.clear();
		//Center
		view.setCenter(Utils::Box2DVectToSfVectPixel(position));
		window.setView(view);

		window.draw(*lvl);
		window.draw(testcar);
		window.draw(testcar2);
		//LOG_DEBUG << checkpointlistener->getCheckpointCount(&testcar);
		


		window.display();
	}

	

	return 0;
}
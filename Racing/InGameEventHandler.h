#pragma once
#include "eventhandler.h"
class InGameEventHandler :
	public EventHandler
{
public:
	InGameEventHandler(void);
	~InGameEventHandler(void);
	virtual bool on_event(const sf::Event &e);
};


#pragma once
#include "stdafx.h"
#include "EventHandler.h"

namespace racing{

std::list<EventHandler*> EVENTS_HANDLERS;
sf::ContextSettings settings;

void init()
{
        settings.antialiasingLevel = 16;
}

}

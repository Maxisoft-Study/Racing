#pragma once
#include "stdafx.h"
#include "eventhandler.h"

class BaseEventHandler :
        public EventHandler
{
public:
        explicit BaseEventHandler(sf::RenderWindow* wptr);
        ~BaseEventHandler(void);
        virtual bool on_event(const sf::Event &e);
protected:
        sf::RenderWindow* window_ptr;
};

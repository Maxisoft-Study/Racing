#pragma once
#include "stdafx.h"

class EventHandler
{

public:
        EventHandler(void);
        virtual ~EventHandler(void);

        virtual bool on_event(const sf::Event &e) = 0;
};

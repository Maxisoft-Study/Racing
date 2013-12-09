#include "stdafx.h"
#include "BaseEventHandler.h"


BaseEventHandler::BaseEventHandler(sf::RenderWindow* wptr) : window_ptr(wptr)
{
}


BaseEventHandler::~BaseEventHandler(void)
{
}

bool BaseEventHandler::on_event(const sf::Event &e)
{

        // on regarde le type de l'évènement...
    switch (e.type)
    {
        // fenêtre fermée
        case sf::Event::Closed:
            this->window_ptr->close();
            return true;



        // touche pressée
        case sf::Event::KeyPressed:


            break;

        // on ne traite pas les autres types d'évènements
        default:
            return false;
    }
        return false;
}

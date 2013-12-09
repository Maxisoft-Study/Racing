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

        // on regarde le type de l'�v�nement...
    switch (e.type)
    {
        // fen�tre ferm�e
        case sf::Event::Closed:
            this->window_ptr->close();
            return true;



        // touche press�e
        case sf::Event::KeyPressed:


            break;

        // on ne traite pas les autres types d'�v�nements
        default:
            return false;
    }
        return false;
}

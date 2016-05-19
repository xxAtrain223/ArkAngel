//
// Created by Anthony on 5/18/2016.
//

#include "PauseState.hpp"

PauseState::PauseState(Engine *engine) :
    engine(engine)
{
    rect.setSize(sf::Vector2f(engine->Window.getSize()));
    rect.setFillColor(sf::Color(0, 0, 0, 127));
}

void PauseState::update()
{

}

void PauseState::draw()
{
    engine->Window.draw(rect);
}

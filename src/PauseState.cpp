//
// Created by Anthony on 5/18/2016.
//

#include "PauseState.hpp"

PauseState::PauseState(Engine *engine) :
    engine(engine)
{
    rect.setSize(sf::Vector2f(engine->Window.getSize()));
    rect.setFillColor(sf::Color(0, 0, 0, 127));

    if (!font.loadFromFile("data/fonts/OpenSans-Regular.ttf"))
        throw "Couldn't find 'data/fonts/OpenSans-Regular.ttf'";
}

void PauseState::update()
{

}

void PauseState::draw()
{
    engine->Window.draw(rect);

    sf::Text text("Paused", font, 20);
    text.setColor(sf::Color::White);
    text.setOrigin(text.getLocalBounds().width, 0);
    text.setPosition(engine->Window.getSize().x - 8, 0);

    engine->Window.draw(text);
}

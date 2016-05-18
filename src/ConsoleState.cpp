//
// Created by Anthony on 5/16/2016.
//

#include "ConsoleState.hpp"

using namespace std;

void ConsoleState::onPush()
{
    engine->Console->Show(true);
}

void ConsoleState::onPop()
{
    engine->Console->Show(false);
}

void ConsoleState::handleEvent(sf::Event event)
{
    engine->ConsoleDesktop.HandleEvent(event);
}

void ConsoleState::update()
{
    if (Clock.getElapsedTime().asMicroseconds() >= 5000)
    {
        engine->ConsoleDesktop.Update(static_cast<float>( Clock.getElapsedTime().asMicroseconds() ) / 1000000.f);
        Clock.restart();
    }

    if (engine->wasKeyPressed(sf::Keyboard::Key::Escape))
        engine->Gsm.pop();
}

//
// Created by Anthony on 5/16/2016.
//

#include "ConsoleState.hpp"

using namespace std;

ConsoleState::ConsoleState(Engine *engine) :
    engine(engine)
{
    engine->Console->Show(true);
    cout << "Constructor" << endl;
}

ConsoleState::~ConsoleState()
{
    //engine->Console->Show(false);
    cout << "Deconstructor" << endl;
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

void ConsoleState::draw()
{  }

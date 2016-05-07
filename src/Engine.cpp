#include "Engine.hpp"

using namespace std;

Engine::Engine() :
    window(sf::VideoMode(800, 600), "ArkAngel"),
    ConsoleDesktop(),
    Console(Terminal::Create()),
    TotalTime(0)
{
    Console->SetTitle("Console");
    ConsoleDesktop.Add(Console);

    window.resetGLStates();
}

void Engine::go()
{
    EngineClock.restart();
    SFGClock.restart();
    while (window.isOpen())
    {
        poll_events();
        update();
        draw();
    }
}

void Engine::poll_events()
{
    sf::Event event;

    while(window.pollEvent(event))
    {
        ConsoleDesktop.HandleEvent(event);

        switch(event.type)
        {
            case sf::Event::Closed:
                window.close();
                break;
        }
    }
}

void Engine::update()
{
    float timeStep = EngineClock.restart().asSeconds();
    TotalTime += timeStep;

    if (SFGClock.getElapsedTime().asMicroseconds() >= 5000)
    {
        ConsoleDesktop.Update(static_cast<float>( SFGClock.getElapsedTime().asMicroseconds() ) / 1000000.f);
        SFGClock.restart();
    }
}

void Engine::draw()
{
    window.clear();
    Sfgui.Display(window);
    window.display();
}

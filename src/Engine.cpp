#include "Engine.hpp"

using namespace std;

Engine::Engine() : shape(100.f)
{
    window.create(sf::VideoMode(800, 600), "ArkAngel");
    shape.setFillColor(sf::Color::Green);
}

void Engine::go()
{
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
}

void Engine::draw()
{
    window.clear();
    window.draw(shape);
    window.display();
}

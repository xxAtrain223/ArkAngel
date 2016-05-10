#include "Engine.hpp"

#include <string>
#include <iostream>

using namespace std;

Engine::Engine() :
    window(sf::VideoMode(800, 600), "ArkAngel"),
    ConsoleDesktop(),
    Console(Terminal::Create()),
    TotalTime(0)
{
    Console->SetTitle("Console");
    ConsoleDesktop.Add(Console);
    ConsoleDesktop.LoadThemeFromFile("data/sfgui.theme");

    window.resetGLStates();
}

void Engine::go()
{
    EngineClock.restart();
    SFGClock.restart();
    while (window.isOpen())
    {
        PrepareOutputBuffers();

        poll_events();
        update();
        draw();

        PrintOutputBuffers();
        ResetOutputBuffers();
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

void Engine::PrepareOutputBuffers() {
    oldOut = std::cout.rdbuf(outBuf.rdbuf());
    oldLog = std::clog.rdbuf(logBuf.rdbuf());
    oldErr = std::cerr.rdbuf(errBuf.rdbuf());
}

void Engine::PrintOutputBuffers() {
    Console->Print(outBuf.str());
    Console->PrintLog(logBuf.str());
    Console->PrintError(errBuf.str());
}

void Engine::ResetOutputBuffers() {
    std::cout.rdbuf(oldOut);
    std::clog.rdbuf(oldLog);
    std::cerr.rdbuf(oldErr);
}

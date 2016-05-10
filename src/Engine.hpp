#ifndef ARKANGEL_ENGINE_HPP
#define ARKANGEL_ENGINE_HPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include "Terminal.hpp"
#include "Print.hpp"

class Engine
{
private:
    sfg::SFGUI Sfgui;
    sfg::Desktop ConsoleDesktop;
    sf::Clock SFGClock;
    Terminal::Ptr Console;

    sf::Clock EngineClock;
    float TotalTime;

    void poll_events();
    void update();
    void draw();

    std::stringstream outBuf;
    std::stringstream logBuf;
    std::stringstream errBuf;

    std::streambuf* oldOut;
    std::streambuf* oldLog;
    std::streambuf* oldErr;

    void PrepareOutputBuffers();
    void PrintOutputBuffers();
    void ResetOutputBuffers();

public:
    sf::RenderWindow window;

    Engine();
    void go();
};

#endif //ARKANGEL_ENGINE_HPP

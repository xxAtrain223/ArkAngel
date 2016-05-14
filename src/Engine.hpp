#ifndef ARKANGEL_ENGINE_HPP
#define ARKANGEL_ENGINE_HPP

#include <assert.h>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <angelscript.h>
#include <scriptstdstring.h>
#include <scriptbuilder.h>
#include <scripthelper.h>

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

    std::stringstream OutBuf;
    std::stringstream LogBuf;
    std::stringstream ErrBuf;

    std::streambuf* OldOut;
    std::streambuf* OldLog;
    std::streambuf* OldErr;

    void PrepareOutputBuffers();
    void PrintOutputBuffers();
    void ResetOutputBuffers();

    asIScriptEngine* ScriptEngine;
    asIScriptModule* ConsoleModule;
    asIScriptContext* ConsoleContext;

    void AsMessageCallback(const asSMessageInfo* msg);

public:
    sf::RenderWindow window;

    Engine();
    void go();
};

#endif //ARKANGEL_ENGINE_HPP

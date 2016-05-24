//
// Created by Anthony on 5/18/2016.
//

#include "ScriptSandbox.hpp"
#include <fstream>

using namespace std;

ScriptSandbox::ScriptSandbox(Engine *engine) :
    engine(engine)
{
    int r;

    engine->ScriptEngine->BeginConfigGroup("ScriptSandbox");
    r = engine->ScriptEngine->RegisterGlobalFunction("void executeFile(string)", asMETHOD(ScriptSandbox, executeFile), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    engine->ScriptEngine->EndConfigGroup();

    ScriptEngine = engine->ScriptEngine;

    engine->Console->Show(true);
}

ScriptSandbox::~ScriptSandbox()
{
    engine->Console->Show(false);

    engine->ScriptEngine->RemoveConfigGroup("ScriptSandbox");
}

void ScriptSandbox::handleEvent(sf::Event event)
{
    engine->ConsoleDesktop.HandleEvent(event);
}

void ScriptSandbox::update()
{
    if (engine->wasKeyPressed(sf::Keyboard::Escape))
    {
        engine->Gsm.pop();
        return;
    }

    if (Clock.getElapsedTime().asMicroseconds() >= 5000)
    {
        engine->ConsoleDesktop.Update(static_cast<float>( Clock.getElapsedTime().asMicroseconds() ) / 1000000.f);
        Clock.restart();
    }
}

void ScriptSandbox::draw()
{
    engine->Window.clear(sf::Color::Red);
}

void ScriptSandbox::executeFile(string filename)
{
    ifstream ifs(filename);
    if (!ifs.good())
    {
        PrintErr("\"%$\" doesn't exist\n", filename);
        return;
    }

    string str((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
    ExecuteString(engine->ScriptEngine, str.c_str());
}
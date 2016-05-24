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
    //engine->ScriptEngine->BeginConfigGroup("ScriptSandbox");
    //r = engine->ScriptEngine->RegisterGlobalFunction("void executeFile(string)", asMETHOD(ScriptSandbox, executeFile), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = engine->ScriptEngine->RegisterGlobalFunction("void executeFile(string)", asMETHODPR(ScriptSandbox, executeFile, (string), void), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    //engine->ScriptEngine->EndConfigGroup();

    ScriptEngine = engine->ScriptEngine;

    engine->Console->Show(true);
}

void ScriptSandbox::onPop()
{
    engine->Console->Show(false);

    //engine->ScriptEngine->RemoveConfigGroup("ScriptSandbox");
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
    string str((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
    if (engine->ScriptEngine == nullptr)
        PrintErr("engine->ScriptEngine is null\n");
    else if (ScriptEngine == nullptr)
        PrintErr("ScriptEngine is null\n");
    else
        ExecuteString(engine->ScriptEngine, str.c_str());
}
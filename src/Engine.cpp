#include "Engine.hpp"

#include <string>
#include <iostream>
#include <regex>

using namespace std;

// Print the script string to the standard output stream
void printfdsafdas(string &msg)
{
    Print(msg.c_str());
}

Engine::Engine() :
    window(sf::VideoMode(800, 600), "ArkAngel"),
    ConsoleDesktop(),
    Console(Terminal::Create()),
    TotalTime(0)
{
    Console->SetTitle("Console");
    ConsoleDesktop.Add(Console);
    ConsoleDesktop.LoadThemeFromFile("data/sfgui.theme");

    ScriptEngine = asCreateScriptEngine();
    int r = ScriptEngine->SetMessageCallback(asMETHOD(Engine, AsMessageCallback), this, asCALL_THISCALL); assert(r >= 0);
    RegisterStdString(ScriptEngine);
    r = ScriptEngine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(printfdsafdas), asCALL_CDECL); assert(r >= 0);

    ConsoleModule = ScriptEngine->GetModule("ConsoleModule", asGM_CREATE_IF_NOT_EXISTS);
    ConsoleContext = ScriptEngine->CreateContext();

    Console->GetSignal(Console->OnCommandEntered).Connect(std::bind([&]{
        string command = Console->GetLastCommand();

        regex rgx("^addvar\\(\"(.*)\"\\);?$");
        smatch match;

        if (std::regex_search(command, match, rgx)) {
            ssub_match matched = match[1];
            string var = matched.str() + ";";
            ConsoleModule->CompileGlobalVar("addvar", var.c_str(), 0);
        }
        else
            ExecuteString(ScriptEngine, command.c_str(), ConsoleModule, ConsoleContext);
    }));


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

    window.setTitle(to_string(1 / timeStep));

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
    OldOut = std::cout.rdbuf(OutBuf.rdbuf());
    OldLog = std::clog.rdbuf(LogBuf.rdbuf());
    OldErr = std::cerr.rdbuf(ErrBuf.rdbuf());
}

void Engine::PrintOutputBuffers() {
    Console->Print(OutBuf.str());
    Console->PrintLog(LogBuf.str());
    Console->PrintError(ErrBuf.str());
}

void Engine::ResetOutputBuffers() {
    OutBuf.str(string());
    LogBuf.str(string());
    ErrBuf.str(string());
    std::cout.rdbuf(OldOut);
    std::clog.rdbuf(OldLog);
    std::cerr.rdbuf(OldErr);
}

void Engine::AsMessageCallback(const asSMessageInfo *msg) {
    string message = PrintStr("%$(%$, %$): %$", msg->section, msg->row, msg->col, msg->message);

    switch(msg->type)
    {
        case asMSGTYPE_ERROR:
            Console->PrintError(message);
            break;
        case asMSGTYPE_WARNING:
            Console->PrintWarning(message);
            break;
        case asMSGTYPE_INFORMATION:
            Console->PrintLog(message);
            break;
    }
}

#include "Engine.hpp"

#include <string>
#include <iostream>
#include <regex>

using namespace std;

// Print the script string to the standard output stream
void printAS(bool b) { Print("%$", b); }
void printAS(int num) { Print("%$", num); }
void printAS(double num) { Print("%$", num); }
void printAS(string& msg) { Print(msg.c_str()); }

Engine::Engine() :
    WindowTitle("ArkAngel"),
    window(sf::VideoMode(800, 600), WindowTitle),
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

    r = ScriptEngine->RegisterGlobalFunction("void print(bool)", asFUNCTIONPR(printAS, (bool), void), asCALL_CDECL); assert(r >= 0);
    r = ScriptEngine->RegisterGlobalFunction("void print(int)", asFUNCTIONPR(printAS, (int), void), asCALL_CDECL); assert(r >= 0);
    r = ScriptEngine->RegisterGlobalFunction("void print(double)", asFUNCTIONPR(printAS, (double), void), asCALL_CDECL); assert(r >= 0);
    r = ScriptEngine->RegisterGlobalFunction("void print(string& in)", asFUNCTIONPR(printAS, (string&), void), asCALL_CDECL); assert(r >= 0);

    r = ScriptEngine->RegisterGlobalFunction("void ShowFPS(bool)", asMETHODPR(Engine, ShowFPS, (bool), void), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = ScriptEngine->RegisterGlobalFunction("void ShowFPS(bool, string)", asMETHODPR(Engine, ShowFPS, (bool, string), void), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);

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

#ifdef NDEBUG
    ShowFPS(false);
#else
    ShowFPS(true);
#endif

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

    CalculateFPS(timeStep);

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
    string outStr = OutBuf.str();
    if (!outStr.empty())
        Console->Print(outStr);

    string logStr = LogBuf.str();
    if (!logStr.empty())
        Console->PrintLog(logStr);

    string errStr = ErrBuf.str();
    if (!errStr.empty())
        Console->PrintError(errStr);
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

void Engine::CalculateFPS(float timeStep)
{
    FPSTimeSinceUpdate += timeStep;
    FPSFrameCount++;

    if (FPSTimeSinceUpdate >= FPSUpdateTime)
    {
        FPS = FPSFrameCount / FPSUpdateTime;
        FPSFrameCount = 0;
        FPSTimeSinceUpdate = 0;

        if (FPSUpdateCallback != nullptr)
            FPSUpdateCallback(FPS);
    }
}

void Engine::ShowFPS(bool show)
{
    ShowFPS(show, "titlebar");
}

void Engine::ShowFPS(bool show, string function)
{
    if (show)
    {
        if (function == "titlebar")
        {
            FPSUpdateCallback = [&](float fps) {
                window.setTitle(PrintStr("%$;  FPS: %.f1$", WindowTitle, FPS));
            };
        }
        else
            PrintErr("'%$' is not an available function for ShowFPS", function);
    }
    else
    {
        window.setTitle(WindowTitle);
        FPSUpdateCallback = nullptr;
    }
}



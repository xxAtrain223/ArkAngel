#include "Engine.hpp"
#include "MainMenu.hpp"
#include "ConsoleState.hpp"
#include "PauseState.hpp"

#include <Box2D/Box2D.h>

#include <string>
#include <iostream>
#include <regex>

using namespace std;

// Print the script string to the standard output stream
void printAS(bool b) { Print("%$\n", b); }
void printAS(int num) { Print("%$\n", num); }
void printAS(double num) { Print("%$\n", num); }
void printAS(string& msg) { Print("%$\n", msg); }

Engine::Engine() :
    WindowTitle(string("ArkAngel")),
    Window(sf::VideoMode(800, 600), WindowTitle),
    ConsoleDesktop(),
    Console(Terminal::Create()),
    TotalTime(0),
    Gsm(this)
{
    ScriptEngine = asCreateScriptEngine();
    ConsoleModule = ScriptEngine->GetModule("ConsoleModule", asGM_CREATE_IF_NOT_EXISTS);

    int r = ScriptEngine->SetMessageCallback(asMETHOD(Engine, AsMessageCallback), this, asCALL_THISCALL); assert(r >= 0);
    RegisterStdString(ScriptEngine);
    RegisterScriptArray(ScriptEngine, true);

    r = ScriptEngine->RegisterGlobalFunction("void print(bool)", asFUNCTIONPR(printAS, (bool), void), asCALL_CDECL); assert(r >= 0);
    r = ScriptEngine->RegisterGlobalFunction("void print(int)", asFUNCTIONPR(printAS, (int), void), asCALL_CDECL); assert(r >= 0);
    r = ScriptEngine->RegisterGlobalFunction("void print(double)", asFUNCTIONPR(printAS, (double), void), asCALL_CDECL); assert(r >= 0);
    r = ScriptEngine->RegisterGlobalFunction("bool print(string)", asFUNCTIONPR(printAS, (string&), void), asCALL_CDECL); assert(r >= 0);

    r = ScriptEngine->RegisterGlobalFunction("void showFPS(bool)", asMETHODPR(Engine, showFPS, (bool), void), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = ScriptEngine->RegisterGlobalFunction("void showFPS(bool, string)", asMETHODPR(Engine, showFPS, (bool, string), void), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = ScriptEngine->RegisterGlobalFunction("void printModules()", asMETHOD(Engine, printModules), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = ScriptEngine->RegisterGlobalFunction("void setConsoleModule(string)", asMETHOD(Engine, setConsoleModule), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);

    r = ScriptEngine->RegisterGlobalFunction("bool isKeyDown(string)", asMETHODPR(Engine, isKeyDown, (const string&), bool), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = ScriptEngine->RegisterGlobalFunction("bool isKeyUp(string)", asMETHODPR(Engine, isKeyUp, (const string&), bool), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = ScriptEngine->RegisterGlobalFunction("bool wasKeyPressed(string)", asMETHODPR(Engine, wasKeyPressed, (const string&), bool), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = ScriptEngine->RegisterGlobalFunction("bool wasKeyReleased(string)", asMETHODPR(Engine, wasKeyReleased, (const string&), bool), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);

    r = ScriptEngine->RegisterGlobalFunction("bool isMouseButtonDown(string)", asMETHODPR(Engine, isMouseButtonDown, (const string&), bool), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = ScriptEngine->RegisterGlobalFunction("bool isMouseButtonUp(string)", asMETHODPR(Engine, isMouseButtonUp, (const string&), bool), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = ScriptEngine->RegisterGlobalFunction("bool wasMouseButtonPressed(string)", asMETHODPR(Engine, wasMouseButtonPressed, (const string&), bool), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = ScriptEngine->RegisterGlobalFunction("bool wasMouseButtonReleased(string)", asMETHODPR(Engine, wasMouseButtonReleased, (const string&), bool), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);

    r = ScriptEngine->RegisterObjectType("MousePosition", sizeof(MousePosition), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS); assert(r >= 0);
    r = ScriptEngine->RegisterObjectProperty("MousePosition", "int x", asOFFSET(MousePosition, x)); assert(r >= 0);
    r = ScriptEngine->RegisterObjectProperty("MousePosition", "int y", asOFFSET(MousePosition, y)); assert(r >= 0);
    r = ScriptEngine->RegisterObjectProperty("MousePosition", "int wheel", asOFFSET(MousePosition, wheel)); assert(r >= 0);
    r = ScriptEngine->RegisterGlobalFunction("MousePosition getMousePosition()", asMETHOD(Engine, getMousePosition), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);

    r = ScriptEngine->RegisterObjectType("b2Body", sizeof(b2Body), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS); assert(r >= 0);
    r = ScriptEngine->RegisterObjectMethod("b2Body", "void SetAngularVelocity(double)", asMETHOD(b2Body, SetAngularVelocity), asCALL_THISCALL); assert(r >= 0);

#ifdef NDEBUG
    showFPS(false);
#else
    showFPS(true);
#endif

    Console->SetTitle("Console");
    ConsoleDesktop.Add(Console);
    ConsoleDesktop.LoadThemeFromFile("data/sfgui.theme");

    Console->GetSignal(Terminal::OnCloseButton).Connect(std::bind([&]{ Gsm.pop(); }));

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
            ExecuteString(ScriptEngine, command.c_str(), ConsoleModule);
    }));

    Console->Show(false);

    sfKeyMap["A"] = sf::Keyboard::Key::A;
    sfKeyMap["B"] = sf::Keyboard::Key::B;
    sfKeyMap["C"] = sf::Keyboard::Key::C;
    sfKeyMap["D"] = sf::Keyboard::Key::D;
    sfKeyMap["E"] = sf::Keyboard::Key::E;
    sfKeyMap["F"] = sf::Keyboard::Key::F;
    sfKeyMap["G"] = sf::Keyboard::Key::G;
    sfKeyMap["H"] = sf::Keyboard::Key::H;
    sfKeyMap["I"] = sf::Keyboard::Key::I;
    sfKeyMap["J"] = sf::Keyboard::Key::J;
    sfKeyMap["K"] = sf::Keyboard::Key::K;
    sfKeyMap["L"] = sf::Keyboard::Key::L;
    sfKeyMap["M"] = sf::Keyboard::Key::M;
    sfKeyMap["N"] = sf::Keyboard::Key::N;
    sfKeyMap["O"] = sf::Keyboard::Key::O;
    sfKeyMap["P"] = sf::Keyboard::Key::P;
    sfKeyMap["Q"] = sf::Keyboard::Key::Q;
    sfKeyMap["R"] = sf::Keyboard::Key::R;
    sfKeyMap["S"] = sf::Keyboard::Key::S;
    sfKeyMap["T"] = sf::Keyboard::Key::T;
    sfKeyMap["U"] = sf::Keyboard::Key::U;
    sfKeyMap["V"] = sf::Keyboard::Key::V;
    sfKeyMap["W"] = sf::Keyboard::Key::W;
    sfKeyMap["X"] = sf::Keyboard::Key::X;
    sfKeyMap["Y"] = sf::Keyboard::Key::Y;
    sfKeyMap["Z"] = sf::Keyboard::Key::Z;
    sfKeyMap["Num0"] = sf::Keyboard::Key::Num0;
    sfKeyMap["Num1"] = sf::Keyboard::Key::Num1;
    sfKeyMap["Num2"] = sf::Keyboard::Key::Num2;
    sfKeyMap["Num3"] = sf::Keyboard::Key::Num3;
    sfKeyMap["Num4"] = sf::Keyboard::Key::Num4;
    sfKeyMap["Num5"] = sf::Keyboard::Key::Num5;
    sfKeyMap["Num6"] = sf::Keyboard::Key::Num6;
    sfKeyMap["Num7"] = sf::Keyboard::Key::Num7;
    sfKeyMap["Num8"] = sf::Keyboard::Key::Num8;
    sfKeyMap["Num9"] = sf::Keyboard::Key::Num9;
    sfKeyMap["Escape"] = sf::Keyboard::Key::Escape;
    sfKeyMap["LControl"] = sf::Keyboard::Key::LControl;
    sfKeyMap["LShift"] = sf::Keyboard::Key::LShift;
    sfKeyMap["LAlt"] = sf::Keyboard::Key::LAlt;
    sfKeyMap["LSystem"] = sf::Keyboard::Key::LSystem;
    sfKeyMap["RControl"] = sf::Keyboard::Key::RControl;
    sfKeyMap["RShift"] = sf::Keyboard::Key::RShift;
    sfKeyMap["RAlt"] = sf::Keyboard::Key::RAlt;
    sfKeyMap["RSystem"] = sf::Keyboard::Key::RSystem;
    sfKeyMap["Menu"] = sf::Keyboard::Key::Menu;
    sfKeyMap["LBracket"] = sf::Keyboard::Key::LBracket;
    sfKeyMap["RBracket"] = sf::Keyboard::Key::RBracket;
    sfKeyMap["SemiColon"] = sf::Keyboard::Key::SemiColon;
    sfKeyMap["Comma"] = sf::Keyboard::Key::Comma;
    sfKeyMap["Period"] = sf::Keyboard::Key::Period;
    sfKeyMap["Quote"] = sf::Keyboard::Key::Quote;
    sfKeyMap["Slash"] = sf::Keyboard::Key::Slash;
    sfKeyMap["BackSlash"] = sf::Keyboard::Key::BackSlash;
    sfKeyMap["Tilde"] = sf::Keyboard::Key::Tilde;
    sfKeyMap["Equal"] = sf::Keyboard::Key::Equal;
    sfKeyMap["Dash"] = sf::Keyboard::Key::Dash;
    sfKeyMap["Space"] = sf::Keyboard::Key::Space;
    sfKeyMap["Return"] = sf::Keyboard::Key::Return;
    sfKeyMap["BackSpace"] = sf::Keyboard::Key::BackSpace;
    sfKeyMap["Tab"] = sf::Keyboard::Key::Tab;
    sfKeyMap["PageUp"] = sf::Keyboard::Key::PageUp;
    sfKeyMap["PageDown"] = sf::Keyboard::Key::PageDown;
    sfKeyMap["End"] = sf::Keyboard::Key::End;
    sfKeyMap["Home"] = sf::Keyboard::Key::Home;
    sfKeyMap["Insert"] = sf::Keyboard::Key::Insert;
    sfKeyMap["Delete"] = sf::Keyboard::Key::Delete;
    sfKeyMap["Add"] = sf::Keyboard::Key::Add;
    sfKeyMap["Subtract"] = sf::Keyboard::Key::Subtract;
    sfKeyMap["Multiply"] = sf::Keyboard::Key::Multiply;
    sfKeyMap["Divide"] = sf::Keyboard::Key::Divide;
    sfKeyMap["Left"] = sf::Keyboard::Key::Left;
    sfKeyMap["Right"] = sf::Keyboard::Key::Right;
    sfKeyMap["Up"] = sf::Keyboard::Key::Up;
    sfKeyMap["Down"] = sf::Keyboard::Key::Down;
    sfKeyMap["Numpad0"] = sf::Keyboard::Key::Numpad0;
    sfKeyMap["Numpad1"] = sf::Keyboard::Key::Numpad1;
    sfKeyMap["Numpad2"] = sf::Keyboard::Key::Numpad2;
    sfKeyMap["Numpad3"] = sf::Keyboard::Key::Numpad3;
    sfKeyMap["Numpad4"] = sf::Keyboard::Key::Numpad4;
    sfKeyMap["Numpad5"] = sf::Keyboard::Key::Numpad5;
    sfKeyMap["Numpad6"] = sf::Keyboard::Key::Numpad6;
    sfKeyMap["Numpad7"] = sf::Keyboard::Key::Numpad7;
    sfKeyMap["Numpad8"] = sf::Keyboard::Key::Numpad8;
    sfKeyMap["Numpad9"] = sf::Keyboard::Key::Numpad9;
    sfKeyMap["F1"] = sf::Keyboard::Key::F1;
    sfKeyMap["F2"] = sf::Keyboard::Key::F2;
    sfKeyMap["F3"] = sf::Keyboard::Key::F3;
    sfKeyMap["F4"] = sf::Keyboard::Key::F4;
    sfKeyMap["F5"] = sf::Keyboard::Key::F5;
    sfKeyMap["F6"] = sf::Keyboard::Key::F6;
    sfKeyMap["F7"] = sf::Keyboard::Key::F7;
    sfKeyMap["F8"] = sf::Keyboard::Key::F8;
    sfKeyMap["F9"] = sf::Keyboard::Key::F9;
    sfKeyMap["F10"] = sf::Keyboard::Key::F10;
    sfKeyMap["F11"] = sf::Keyboard::Key::F11;
    sfKeyMap["F12"] = sf::Keyboard::Key::F12;
    sfKeyMap["F13"] = sf::Keyboard::Key::F13;
    sfKeyMap["F14"] = sf::Keyboard::Key::F14;
    sfKeyMap["F15"] = sf::Keyboard::Key::F15;
    sfKeyMap["Pause"] = sf::Keyboard::Key::Pause;

    sfButtonMap["Left"] = sf::Mouse::Button::Left;
    sfButtonMap["Right"] = sf::Mouse::Button::Right;
    sfButtonMap["Middle"] = sf::Mouse::Button::Middle;
    sfButtonMap["XButton1"] = sf::Mouse::Button::XButton1;
    sfButtonMap["XButton2"] = sf::Mouse::Button::XButton2;
}

void Engine::go()
{
    Gsm.push(StateMaker<MainMenu>(this));

    EngineClock.restart();

    while (Window.isOpen() && !Gsm.empty())
    {
        ++currentTick;
        PrepareOutputBuffers();

        poll_events();
        int c = Gsm.size();
        if (c > 0)
            update();
        if (Gsm.size() == c)
            draw();

        PrintOutputBuffers();
        ResetOutputBuffers();
    }
}

void Engine::poll_events()
{
    sf::Event event;
    lastMousePosition = mousePosition;

    while(Window.pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::Closed:
                while (!Gsm.empty())
                    Gsm.pop();
                break;
            case sf::Event::KeyPressed:
                if (event.key.code != sf::Keyboard::Key::Unknown)
                    keyboard[event.key.code].lastPressed = currentTick;
                break;
            case sf::Event::KeyReleased:
                if (event.key.code != sf::Keyboard::Key::Unknown)
                    keyboard[event.key.code].lastReleased = currentTick;
                break;
            case sf::Event::MouseButtonPressed:
                mouseButtons[event.mouseButton.button].lastPressed = currentTick;
                break;
            case sf::Event::MouseButtonReleased:
                mouseButtons[event.mouseButton.button].lastReleased = currentTick;
                break;
            case sf::Event::MouseWheelMoved:
                mousePosition.wheel += event.mouseWheel.delta;
                break;
            case sf::Event::MouseMoved:
                mousePosition.x = event.mouseMove.x;
                mousePosition.y = event.mouseMove.y;
                break;
        }

        Gsm.handleEvent(event);
    }
}

void Engine::update()
{
    float timeStep = EngineClock.restart().asSeconds();
    TotalTime += timeStep;

    CalculateFPS(timeStep);

    if (wasKeyPressed(sf::Keyboard::Key::Tilde) && !Console->IsLocallyVisible()) {
        Gsm.push(StateMaker<PauseState>(this));
        Gsm.push(StateMaker<ConsoleState>(this));
        return;
    }

    Gsm.update();
}

void Engine::draw()
{
    Window.clear();
    Gsm.draw();
    Sfgui.Display(Window);
    Window.display();
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

void Engine::showFPS(bool show)
{
    showFPS(show, "titlebar");
}

void Engine::showFPS(bool show, string function)
{
    if (show)
    {
        if (function == "titlebar")
        {
            FPSUpdateCallback = [&](float fps) {
                Window.setTitle(PrintStr("%$;  FPS: %.f1$", WindowTitle, FPS));
            };
        }
        else
            PrintErr("'%$' is not an available function for ShowFPS", function);
    }
    else
    {
        Window.setTitle(WindowTitle);
        FPSUpdateCallback = nullptr;
    }
}

bool Engine::isKeyDown(sf::Keyboard::Key key)
{
    return keyboard[key].lastPressed > keyboard[key].lastReleased;
}

bool Engine::isKeyUp(sf::Keyboard::Key key)
{
    return !isKeyDown(key);
}

bool Engine::wasKeyPressed(sf::Keyboard::Key key)
{
    return keyboard[key].lastPressed == currentTick;
}

bool Engine::wasKeyReleased(sf::Keyboard::Key key)
{
    return keyboard[key].lastReleased == currentTick;
}

bool Engine::isKeyDown(const std::string& key)
{
    return isKeyDown(sfKeyMap.at(key));
}

bool Engine::isKeyUp(const std::string& key)
{
    return isKeyUp(sfKeyMap.at(key));
}

bool Engine::wasKeyPressed(const std::string& key)
{
    return wasKeyPressed(sfKeyMap.at(key));
}

bool Engine::wasKeyReleased(const std::string& key)
{
    return wasKeyReleased(sfKeyMap.at(key));
}

bool Engine::isMouseButtonDown(sf::Mouse::Button button)
{
    return mouseButtons[button].lastPressed > mouseButtons[button].lastReleased;
}

bool Engine::isMouseButtonUp(sf::Mouse::Button button)
{
    return !isMouseButtonDown(button);
}

bool Engine::wasMouseButtonPressed(sf::Mouse::Button button)
{
    return mouseButtons[button].lastPressed == currentTick;
}

bool Engine::wasMouseButtonReleased(sf::Mouse::Button button)
{
    return mouseButtons[button].lastReleased == currentTick;
}

bool Engine::isMouseButtonDown(const std::string& button)
{
    return isMouseButtonDown(sfButtonMap.at(button));
}

bool Engine::isMouseButtonUp(const std::string& button)
{
    return isMouseButtonUp(sfButtonMap.at(button));
}

bool Engine::wasMouseButtonPressed(const std::string& button)
{
    return wasMouseButtonPressed(sfButtonMap.at(button));
}

bool Engine::wasMouseButtonReleased(const std::string& button)
{
    return wasMouseButtonReleased(sfButtonMap.at(button));
}

const MousePosition Engine::getMousePosition() const
{
    return mousePosition;
}

const MousePosition Engine::getMousePositionDelta() const
{
    MousePosition pos;
    pos.x = lastMousePosition.x - mousePosition.x;
    pos.y = lastMousePosition.y - mousePosition.y;
    pos.wheel = lastMousePosition.wheel - mousePosition.wheel;
    return pos;
}

void Engine::printModules()
{
    asUINT modCount = ScriptEngine->GetModuleCount();

    for (asUINT i = 0; i < modCount; i++)
        Print("%$\n", ScriptEngine->GetModuleByIndex(i)->GetName());
}

void Engine::setConsoleModule(string moduleName)
{
    asIScriptModule* module = ScriptEngine->GetModule(moduleName.c_str());
    if (module == nullptr)
        PrintErr("\"%$\" module does not exist.", moduleName);
    else
        ConsoleModule = module;
}

const bool Engine::hasMouseMoved() const
{
    return (lastMousePosition.x != mousePosition.x) || (lastMousePosition.y != mousePosition.y);
}

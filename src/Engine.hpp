#ifndef ARKANGEL_ENGINE_HPP
#define ARKANGEL_ENGINE_HPP

#include <assert.h>
#include <vector>
#include <unordered_map>

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
#include "GameStateManager.hpp"

struct MousePosition
{
    int x;
    int y;
    int wheel = 0;
};

class Engine
{
private:
    std::string WindowTitle;
    sfg::SFGUI Sfgui;

    sf::Clock EngineClock;
    float TotalTime;

    float FPSUpdateTime = 0.25;
    float FPSTimeSinceUpdate = 0;
    float FPS = 0;
    float FPSFrameCount = 0;
    std::function<void(float)> FPSUpdateCallback = nullptr;

    asIScriptModule* ConsoleModule;
    asIScriptContext* ConsoleContext;

    void ShowFPS(bool show);
    void ShowFPS(bool show, std::string method);

    void CalculateFPS(float timeStep);

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

    void AsMessageCallback(const asSMessageInfo* msg);

    struct KeyState
    {
        int lastPressed = -1;
        int lastReleased = -1;
    };

    struct MouseButtonState
    {
        int lastPressed = -1;
        int lastReleased = -1;
    };

    int currentTick = 0;
    std::vector<KeyState> keyboard = std::vector<KeyState>(sf::Keyboard::KeyCount);
    std::vector<MouseButtonState> mouseButtons = std::vector<MouseButtonState>(sf::Mouse::ButtonCount);
    MousePosition mousePosition;

    std::unordered_map<std::string, sf::Keyboard::Key> sfKeyMap;
    std::unordered_map<std::string, sf::Mouse::Button> sfButtonMap;

public:
    sf::RenderWindow Window;

    asIScriptEngine* ScriptEngine;

    GameStateManager Gsm;

    sfg::Desktop ConsoleDesktop;
    Terminal::Ptr Console;

    Engine();
    void go();

    std::string GetWindowTitle() { return WindowTitle; }

    bool isKeyDown(sf::Keyboard::Key key);
    bool isKeyUp(sf::Keyboard::Key key);
    bool wasKeyPressed(sf::Keyboard::Key key);
    bool wasKeyReleased(sf::Keyboard::Key key);

    bool isKeyDown(const std::string& key);
    bool isKeyUp(const std::string& key);
    bool wasKeyPressed(const std::string& key);
    bool wasKeyReleased(const std::string& key);

    bool isMouseButtonDown(sf::Mouse::Button button);
    bool isMouseButtonUp(sf::Mouse::Button button);
    bool wasMouseButtonPressed(sf::Mouse::Button button);
    bool wasMouseButtonReleased(sf::Mouse::Button button);

    bool isMouseButtonDown(const std::string& button);
    bool isMouseButtonUp(const std::string& button);
    bool wasMouseButtonPressed(const std::string& button);
    bool wasMouseButtonReleased(const std::string& button);

    const MousePosition getMousePosition() const;
};

#endif //ARKANGEL_ENGINE_HPP

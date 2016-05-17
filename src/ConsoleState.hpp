//
// Created by Anthony on 5/16/2016.
//

#ifndef ARKANGEL_CONSOLESTATE_HPP
#define ARKANGEL_CONSOLESTATE_HPP

#include "Engine.hpp"

#include <sfml/System.hpp>

class ConsoleState {
    Engine *engine = nullptr;

    sf::Clock Clock;

public:
    ConsoleState() = default;
    ConsoleState(Engine *engine);
    ConsoleState(const ConsoleState&) = default;               // Copy constructor
    ConsoleState(ConsoleState&& other) = default;              // Move constructor
    ConsoleState& operator=(const ConsoleState&) & = default;  // Copy assignment operator
    ConsoleState& operator=(ConsoleState&&) & = default;       // Move assignment operator
    ~ConsoleState();

    bool haltsHandleEvent() { return true; }
    bool haltsUpdate() { return true; }
    bool haltsDraw() { return false; }
    void handleEvent(sf::Event event);
    void update();
    void draw();
};


#endif //ARKANGEL_CONSOLESTATE_HPP

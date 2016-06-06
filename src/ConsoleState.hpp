//
// Created by Anthony on 5/16/2016.
//

#ifndef ARKANGEL_CONSOLESTATE_HPP
#define ARKANGEL_CONSOLESTATE_HPP

#include "Engine.hpp"

#include <sfml/System.hpp>

class ConsoleState {
    Engine *engine = nullptr;
    const std::string SUID;

    sf::Clock Clock;

public:
    ConsoleState() = default;
    ConsoleState(std::string suid, Engine *engine);
    ~ConsoleState();
    bool haltsHandleEvent() { return true; }
    bool haltsUpdate() { return false; }
    bool haltsDraw() { return false; }
    void handleEvent(sf::Event event);
    void update();
    void draw() {}
    const std::string getSUID() { return SUID; }
    void onPause() {}
    void onResume() {}
};


#endif //ARKANGEL_CONSOLESTATE_HPP

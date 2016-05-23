//
// Created by Anthony on 5/18/2016.
//

#ifndef ARKANGEL_SCRIPTPLAYGROUND_HPP
#define ARKANGEL_SCRIPTPLAYGROUND_HPP

#include "Engine.hpp"

class ScriptSandbox {
    Engine* engine;

public:
    ScriptSandbox() = default;
    ScriptSandbox(Engine *engine);
    bool haltsHandleEvent() { return true; }
    bool haltsUpdate() { return true; }
    bool haltsDraw() { return true; }
    void onPush() {}
    void onPop();
    void handleEvent(sf::Event event) {}
    void update();
    void draw();
};


#endif //ARKANGEL_SCRIPTPLAYGROUND_HPP

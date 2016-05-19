//
// Created by Anthony on 5/18/2016.
//

#ifndef ARKANGEL_PAUSESTATE_HPP
#define ARKANGEL_PAUSESTATE_HPP

#include "Engine.hpp"

#include <sfml/Graphics.hpp>

class PauseState {
    Engine* engine;

    sf::RectangleShape rect;

public:
    PauseState() = default;
    PauseState(Engine *engine);
    bool haltsHandleEvent() { return true; }
    bool haltsUpdate() { return true; }
    bool haltsDraw() { return false; }
    void onPush() {}
    void onPop() {}
    void handleEvent(sf::Event event) {}
    void update();
    void draw();
};

#endif //ARKANGEL_PAUSESTATE_HPP

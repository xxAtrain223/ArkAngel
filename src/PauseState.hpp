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
    sf::Font font;

public:
    PauseState() = default;
    PauseState(Engine *engine);
    ~PauseState() = default;
    bool haltsHandleEvent() { return true; }
    bool haltsUpdate() { return true; }
    bool haltsDraw() { return false; }
    void handleEvent(sf::Event event) {}
    void update();
    void draw();
};

#endif //ARKANGEL_PAUSESTATE_HPP

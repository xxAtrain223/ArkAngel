//
// Created by Anthony on 5/18/2016.
//

#ifndef ARKANGEL_PAUSESTATE_HPP
#define ARKANGEL_PAUSESTATE_HPP

#include "Engine.hpp"

#include <sfml/Graphics.hpp>

class PauseState {
    Engine* engine;
    const std::string SUID;

    sf::RectangleShape rect;
    sf::Font font;

public:
    PauseState() = default;
    PauseState(std::string suid, Engine *engine);
    bool haltsHandleEvent() { return true; }
    bool haltsUpdate() { return true; }
    bool haltsDraw() { return false; }
    void handleEvent(sf::Event event) {}
    void update();
    void draw();
    const std::string getSUID() { return SUID; }
    void onPause() {}
    void onResume() {}
};

#endif //ARKANGEL_PAUSESTATE_HPP

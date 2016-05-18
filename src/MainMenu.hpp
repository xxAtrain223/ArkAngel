//
// Created by Anthony on 5/16/2016.
//

#ifndef ARKANGEL_MAINMENU_HPP
#define ARKANGEL_MAINMENU_HPP

#include "Engine.hpp"

#include <sfml/Graphics.hpp>

#include <vector>
#include <utility>

class MainMenu {
    Engine *engine = nullptr;

    sf::Font optionFont;

    std::vector<std::pair<std::string, std::function<StateErasure()>>> items;
    int selected = 0;

public:
    MainMenu() = default;
    MainMenu(Engine *engine);
    bool haltsHandleEvent() { return true; }
    bool haltsUpdate() { return true; }
    bool haltsDraw() { return true; }
    void onPush() {}
    void onPop() {}
    void handleEvent(sf::Event event) {}
    void update();
    void draw();
};

#endif //ARKANGEL_MAINMENU_HPP

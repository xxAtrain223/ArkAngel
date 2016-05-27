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
    const std::string SUID;

    sf::Font optionFont;

    std::vector<std::pair<std::string, std::function<StateErasure(std::string)>>> items;
    int selected = 0;

public:
    MainMenu() = default;
    MainMenu(std::string suid, Engine *engine);
    bool haltsHandleEvent() { return true; }
    bool haltsUpdate() { return true; }
    bool haltsDraw() { return true; }
    void handleEvent(sf::Event event) {}
    void update();
    void draw();
    const std::string getSUID() { return SUID; }
};

#endif //ARKANGEL_MAINMENU_HPP

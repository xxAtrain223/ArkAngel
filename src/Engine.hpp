#ifndef ARKANGEL_ENGINE_HPP
#define ARKANGEL_ENGINE_HPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Engine
{
private:
    sf::CircleShape shape;

    void poll_events();
    void update();
    void draw();

public:
    sf::RenderWindow window;

    Engine();
    void go();
};

#endif //ARKANGEL_ENGINE_HPP

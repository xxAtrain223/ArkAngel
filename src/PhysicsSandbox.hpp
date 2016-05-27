//
// Created by Anthony on 5/27/2016.
//

#ifndef ARKANGEL_PHYSICSSANDBOX_HPP
#define ARKANGEL_PHYSICSSANDBOX_HPP

#include "Engine.hpp"
#include "Box2dDebugDraw.hpp"

#include <Box2D/Box2D.h>
#include <sfml/Graphics.hpp>

class PhysicsSandbox
{
    Engine *engine = nullptr;
    const std::string SUID;

    b2World* world;
    Box2dDebugDraw* debugDraw;
    float timeStep;
    int velIter;
    int posIter;

    sf::Clock clock;
    float accumulator;

public:
    PhysicsSandbox() = default;
    PhysicsSandbox(std::string suid, Engine *engine);
    bool haltsHandleEvent() { return true; }
    bool haltsUpdate() { return true; }
    bool haltsDraw() { return true; }
    void handleEvent(sf::Event event) {}
    void update();
    void draw();
    const std::string getSUID() { return SUID; }
};


#endif //ARKANGEL_PHYSICSSANDBOX_HPP

//
// Created by Anthony on 5/27/2016.
//

#include "PhysicsSandbox.hpp"

PhysicsSandbox::PhysicsSandbox(std::string suid, Engine *engine) :
    engine(engine),
    SUID(suid)
{
    world = new b2World(b2Vec2(0.f, -9.81f * 8));
    debugDraw = new Box2dDebugDraw(engine->Window, 32.f);
    debugDraw->SetFlags(b2Draw::e_shapeBit);
    world->SetDebugDraw(debugDraw);
    timeStep = 1.f / 60.f;
    velIter = 8;
    posIter = 3;
    accumulator = 0;
}

void PhysicsSandbox::update()
{
    if (engine->wasKeyPressed(sf::Keyboard::Escape))
    {
        engine->Gsm.pop();
        return;
    }

    sf::Time t = clock.restart();
    accumulator += t.asSeconds();
    while (accumulator >= timeStep)
    {
        world->Step(timeStep, velIter, posIter);
        accumulator -= timeStep;
    }
}

void PhysicsSandbox::draw()
{
    world->DrawDebugData();
}

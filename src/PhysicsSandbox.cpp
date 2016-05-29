//
// Created by Anthony on 5/27/2016.
//

#include "PhysicsSandbox.hpp"
#include "Util.hpp"

PhysicsSandbox::PhysicsSandbox(std::string suid, Engine *engine) :
    engine(engine),
    SUID(suid)
{
    windowSize = sf::Vector2f(engine->Window.getSize().x, engine->Window.getSize().y);
    cam.setCenter(0, 0);
    cam.setSize(windowSize.x, windowSize.y);

    world = new b2World(b2Vec2(0.f, -9.81f * 4));
    debugDraw = new Box2dDebugDraw(engine->Window, 12.f);
    debugDraw->SetFlags(b2Draw::e_shapeBit);
    world->SetDebugDraw(debugDraw);
    timeStep = 1.f / 60.f;
    velIter = 8;
    posIter = 3;
    accumulator = 0;

    int32 m_fixtureCount = 0;
    b2Timer timer;

    {
        float32 a = 0.5f;
        b2BodyDef bd;
        bd.position.y = -a;
        b2Body* ground = world->CreateBody(&bd);

#if 1
        int32 N = 200;
        int32 M = 10;
        b2Vec2 position;
        position.y = 0.0f;
        for (int32 j = 0; j < M; ++j)
        {
            position.x = -N * a;
            for (int32 i = 0; i < N; ++i)
            {
                b2PolygonShape shape;
                shape.SetAsBox(a, a, position, 0.0f);
                ground->CreateFixture(&shape, 0.0f);
                ++m_fixtureCount;
                position.x += 2.0f * a;
            }
            position.y -= 2.0f * a;
        }
#else
        int32 N = 200;
        int32 M = 10;
        b2Vec2 position;
        position.x = -N * a;
        for (int32 i = 0; i < N; ++i)
        {
            position.y = 0.0f;
            for (int32 j = 0; j < M; ++j)
            {
                b2PolygonShape shape;
                shape.SetAsBox(a, a, position, 0.0f);
                ground->CreateFixture(&shape, 0.0f);
                position.y -= 2.0f * a;
            }
            position.x += 2.0f * a;
        }
#endif
    }

    {
        float32 a = 0.5f;
        b2PolygonShape shape;
        shape.SetAsBox(a, a);

        b2Vec2 x(-7.0f, 0.75f);
        b2Vec2 y;
        b2Vec2 deltaX(0.5625f, 1.25f);
        b2Vec2 deltaY(1.125f, 0.0f);

        for (int32 i = 0; i < 20; ++i)
        {
            y = x;

            for (int32 j = i; j < 20; ++j)
            {
                b2BodyDef bd;
                bd.type = b2_dynamicBody;
                bd.position = y;

                //if (i == 0 && j == 0)
                //{
                //	bd.allowSleep = false;
                //}
                //else
                //{
                //	bd.allowSleep = true;
                //}

                b2Body* body = world->CreateBody(&bd);
                body->CreateFixture(&shape, 5.0f);
                ++m_fixtureCount;
                y += deltaY;
            }

            x += deltaX;
        }
    }
}

void PhysicsSandbox::update()
{
    if (engine->wasKeyPressed(sf::Keyboard::Escape))
    {
        engine->Gsm.pop();
        return;
    }

    MousePosition delta = engine->getMousePositionDelta();

    if (engine->isMouseButtonDown(sf::Mouse::Right))
        cam.move(sf::Vector2f(delta.x, delta.y) / (windowSize.x / cam.getSize().x));
    if (delta.wheel != 0)
        cam.zoom(1 + delta.wheel * 0.1f);

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
    engine->Window.setView(cam);
    world->DrawDebugData();
    engine->Window.setView(engine->Window.getDefaultView());
}

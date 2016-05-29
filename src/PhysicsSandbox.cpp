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

    world = new b2World(b2Vec2(0.f, -9.81f * 1));
    debugDraw = new Box2dDebugDraw(engine->Window, 12.f);
    debugDraw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit);
    world->SetDebugDraw(debugDraw);
    timeStep = 1.f / 120.f;
    velIter = 8;
    posIter = 3;
    accumulator = 0;

    b2Body* ground = NULL;
    {
        b2BodyDef bd;
        ground = world->CreateBody(&bd);

        b2EdgeShape shape;
        shape.Set(b2Vec2(-40.0f, 0.0f), b2Vec2(40.0f, 0.0f));
        ground->CreateFixture(&shape, 0.0f);
    }

    {
        b2Body* prevBody = ground;

        // Define crank.
        {
            b2PolygonShape shape;
            shape.SetAsBox(0.5f, 2.0f);

            b2BodyDef bd;
            bd.type = b2_dynamicBody;
            bd.position.Set(0.0f, 7.0f);
            b2Body* body = world->CreateBody(&bd);
            body->CreateFixture(&shape, 2.0f);

            b2RevoluteJointDef rjd;
            rjd.Initialize(prevBody, body, b2Vec2(0.0f, 5.0f));
            rjd.motorSpeed = 1.0f * b2_pi;
            rjd.maxMotorTorque = 10000.0f;
            rjd.enableMotor = true;
            b2RevoluteJoint* m_joint1 = (b2RevoluteJoint*)world->CreateJoint(&rjd);

            prevBody = body;
        }

        // Define follower.
        {
            b2PolygonShape shape;
            shape.SetAsBox(0.5f, 4.0f);

            b2BodyDef bd;
            bd.type = b2_dynamicBody;
            bd.position.Set(0.0f, 13.0f);
            b2Body* body = world->CreateBody(&bd);
            body->CreateFixture(&shape, 2.0f);

            b2RevoluteJointDef rjd;
            rjd.Initialize(prevBody, body, b2Vec2(0.0f, 9.0f));
            rjd.enableMotor = false;
            world->CreateJoint(&rjd);

            prevBody = body;
        }

        // Define piston
        {
            b2PolygonShape shape;
            shape.SetAsBox(1.5f, 1.5f);

            b2BodyDef bd;
            bd.type = b2_dynamicBody;
            bd.fixedRotation = true;
            bd.position.Set(0.0f, 17.0f);
            b2Body* body = world->CreateBody(&bd);
            body->CreateFixture(&shape, 2.0f);

            b2RevoluteJointDef rjd;
            rjd.Initialize(prevBody, body, b2Vec2(0.0f, 17.0f));
            world->CreateJoint(&rjd);

            b2PrismaticJointDef pjd;
            pjd.Initialize(ground, body, b2Vec2(0.0f, 17.0f), b2Vec2(0.0f, 1.0f));

            pjd.maxMotorForce = 1000.0f;
            pjd.enableMotor = true;

            b2PrismaticJoint* m_joint2 = (b2PrismaticJoint*)world->CreateJoint(&pjd);
        }

        // Create a payload
        {
            b2PolygonShape shape;
            shape.SetAsBox(1.5f, 1.5f);

            b2BodyDef bd;
            bd.type = b2_dynamicBody;
            bd.position.Set(0.0f, 23.0f);
            b2Body* body = world->CreateBody(&bd);
            body->CreateFixture(&shape, 2.0f);
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
    engine->Window.clear(sf::Color(76, 76, 76));
    engine->Window.setView(cam);
    world->DrawDebugData();
    engine->Window.setView(engine->Window.getDefaultView());
}

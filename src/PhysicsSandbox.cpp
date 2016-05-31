//
// Created by Anthony on 5/27/2016.
//

#include "PhysicsSandbox.hpp"
#include "QueryCallback.hpp"
#include "Util.hpp"

#include <vector>

using namespace std;

PhysicsSandbox::PhysicsSandbox(std::string suid, Engine *engine) :
    engine(engine),
    SUID(suid)
{
    sf::Vector2f windowSize = sf::Vector2f(engine->Window.getSize().x, engine->Window.getSize().y);
    cam.setCenter(0, 0);
    cam.setSize(windowSize.x, windowSize.y);

    string errMessage;
    b2dJson json;
    world = json.readFromFile("data/b2worlds/truck-min.json", errMessage);
    if (errMessage.size() > 0)
    {
        fprintf(stderr, "%s\n", errMessage.c_str());
        fflush(stderr);
    }

    json.getBodiesByName("truckwheel", wheels);
    json.getBodiesByName("truckchassis", chassis);

    //world = new b2World(b2Vec2(0.f, -9.81f * 1));
    debugDraw = new Box2dDebugDraw(engine->Window, 12.f);
    debugDraw->SetFlags(b2Draw::e_shapeBit);
    world->SetDebugDraw(debugDraw);
    timeStep = 1.f / 120.f;
    velIter = 8;
    posIter = 3;
    accumulator = 0;

    b2BodyDef bodyDef;
    groundBody = world->CreateBody(&bodyDef);

#if 0
#if 0

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

#else

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
#endif
#endif
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
        cam.move(sf::Vector2f(delta.x, delta.y) / (engine->Window.getSize().x / cam.getSize().x));
    if (delta.wheel != 0)
        cam.zoom(1 + delta.wheel * 0.1f);

    if (engine->wasMouseButtonPressed(sf::Mouse::Left))
    {
        b2Vec2 p = getWorldPos(engine->getMousePosition());

        if (mouseJoint == NULL)
        {
            b2AABB aabb;
            b2Vec2 d;
            d.Set(0.001f, 0.001f);
            aabb.lowerBound = p - d;
            aabb.upperBound = p + d;

            QueryCallback callback(p);
            world->QueryAABB(&callback, aabb);

            if (callback.m_fixture)
            {
                b2Body *body = callback.m_fixture->GetBody();
                b2MouseJointDef md;
                md.bodyA = groundBody;
                md.bodyB = body;
                md.target = p;
                md.maxForce = 1000.f * body->GetMass();
                mouseJoint = (b2MouseJoint *) world->CreateJoint(&md);
                body->SetAwake(true);
            }
        }
    }

    if (engine->wasMouseButtonReleased(sf::Mouse::Left))
    {
        if (mouseJoint)
        {
            world->DestroyJoint(mouseJoint);
            mouseJoint = NULL;
        }
    }

    if (engine->hasMouseMoved())
        if (mouseJoint)
            mouseJoint->SetTarget(getWorldPos(engine->getMousePosition()));

    float maxSpeed = 20;
    float desiredSpeed = 0;

    if (engine->isKeyDown(sf::Keyboard::A))
        desiredSpeed += maxSpeed;
    if (engine->isKeyDown(sf::Keyboard::D))
        desiredSpeed -= maxSpeed;

    if (desiredSpeed != 0)
    {
        for (auto b : wheels)
            b->SetAngularVelocity(desiredSpeed);

        chassis[0]->ApplyTorque(-125 * Sign(desiredSpeed), true);
    }

    if (mouseJoint == NULL)
    {
        b2Vec2 chassisPos = chassis[0]->GetPosition();
        cam.setCenter(chassisPos.x * debugDraw->getPixelsPerMeter(),
                      chassisPos.y * debugDraw->getPixelsPerMeter() * -1);
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
    engine->Window.clear(sf::Color(76, 76, 76));
    engine->Window.setView(cam);

    world->DrawDebugData();
    if (mouseJoint)
    {
        b2Vec2 p1 = mouseJoint->GetAnchorB();
        b2Vec2 p2 = mouseJoint->GetTarget();

        b2Color c;
        c.Set(0.8f, 0.8f, 0.8f);
        debugDraw->DrawSegment(p1, p2, c);
    }

    engine->Window.setView(engine->Window.getDefaultView());
}

b2Vec2 PhysicsSandbox::getWorldPos(sf::Vector2i p)
{
    sf::Vector2f sfPos = engine->Window.mapPixelToCoords(p, cam);
    float metersPerPixel = debugDraw->getMetersPerPixel();
    return {sfPos.x * metersPerPixel, sfPos.y * metersPerPixel * -1};
}

b2Vec2 PhysicsSandbox::getWorldPos(MousePosition p)
{
    return getWorldPos(sf::Vector2i(p.x, p.y));
}
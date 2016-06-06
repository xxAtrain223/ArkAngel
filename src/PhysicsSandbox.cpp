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
    SUID(suid),
    debugDraw(engine->Window, 12.f)
{
    timeStep = 1.f / 60.f;
    velIter = 8;
    posIter = 3;
    accumulator = 0;

    int r;
    r = engine->ScriptEngine->RegisterGlobalFunction("void loadB2World(string)", asMETHOD(PhysicsSandbox, loadB2World), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = engine->ScriptEngine->RegisterGlobalFunction("float getTimeStep()", asMETHOD(PhysicsSandbox, getTimeStep), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = engine->ScriptEngine->RegisterGlobalFunction("void setTimeStep(float)", asMETHOD(PhysicsSandbox, setTimeStep), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = engine->ScriptEngine->RegisterGlobalFunction("int getVelocityIterations()", asMETHOD(PhysicsSandbox, getVelocityIterations), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = engine->ScriptEngine->RegisterGlobalFunction("void setVelocityIterations(int)", asMETHOD(PhysicsSandbox, setVelocityIterations), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = engine->ScriptEngine->RegisterGlobalFunction("int getPositionIterations()", asMETHOD(PhysicsSandbox, getPositionIterations), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = engine->ScriptEngine->RegisterGlobalFunction("void setPositionIterations(int)", asMETHOD(PhysicsSandbox, setPositionIterations), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = engine->ScriptEngine->RegisterGlobalFunction("void setCameraPosition(float, float)", asMETHOD(PhysicsSandbox, setCameraPosition), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = engine->ScriptEngine->RegisterGlobalFunction("void setCameraZoom(float)", asMETHOD(PhysicsSandbox, setCameraZoom), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);

    /*
    r = engine->ScriptEngine->RegisterObjectType("b2Body", sizeof(b2Body), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS); assert(r >= 0);
    r = engine->ScriptEngine->RegisterGlobalFunction("void SetAngularVelocity(double)", asMETHOD(b2Body, SetAngularVelocity), asCALL_, this); assert(r >= 0);
    r = engine->ScriptEngine->RegisterGlobalFunction("array<b2Body>@ getBodiesByName(string)", asMETHOD(PhysicsSandbox, getBodiesByName), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    */

    clock.start();
}

PhysicsSandbox::~PhysicsSandbox()
{
    delete world;
}

void PhysicsSandbox::update()
{
    if (engine->wasKeyPressed(sf::Keyboard::Escape))
    {
        engine->Gsm.pop();
        return;
    }

    if (world)
    {
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

        if (engine->hasMouseMoved()) if (mouseJoint)
            mouseJoint->SetTarget(getWorldPos(engine->getMousePosition()));


        sf::Time t = clock.getElapsedTime();
        clock.restart();
        accumulator += t.asSeconds();
        while (accumulator >= timeStep)
        {
            world->Step(timeStep, velIter, posIter);
            accumulator -= timeStep;
        }
    }
}

void PhysicsSandbox::draw()
{
    engine->Window.clear(sf::Color(76, 76, 76));
    engine->Window.setView(cam);

    if (world)
    {
        world->DrawDebugData();
        if (mouseJoint)
        {
            b2Vec2 p1 = mouseJoint->GetAnchorB();
            b2Vec2 p2 = mouseJoint->GetTarget();

            b2Color c;
            c.Set(0.8f, 0.8f, 0.8f);
            debugDraw.DrawSegment(p1, p2, c);
        }
    }

    engine->Window.setView(engine->Window.getDefaultView());
}

b2Vec2 PhysicsSandbox::getWorldPos(sf::Vector2i p)
{
    sf::Vector2f sfPos = engine->Window.mapPixelToCoords(p, cam);
    float metersPerPixel = debugDraw.getMetersPerPixel();
    return {sfPos.x * metersPerPixel, sfPos.y * metersPerPixel * -1};
}

b2Vec2 PhysicsSandbox::getWorldPos(MousePosition p)
{
    return getWorldPos(sf::Vector2i(p.x, p.y));
}

void PhysicsSandbox::loadB2World(std::string filename)
{
    if (world)
        delete world;

    string errMessage;
    world = json.readFromFile(filename.c_str(), errMessage);
    if (errMessage.size() > 0)
    {
        PrintErr("%$\n", errMessage);
        return;
    }

    debugDraw.SetFlags(b2Draw::e_shapeBit);
    world->SetDebugDraw(&debugDraw);

    b2BodyDef bodyDef;
    groundBody = world->CreateBody(&bodyDef);

    sf::Vector2f windowSize = sf::Vector2f(engine->Window.getSize().x, engine->Window.getSize().y);
    cam.setCenter(0, 0);
    cam.setSize(windowSize.x, windowSize.y);
}

float PhysicsSandbox::getTimeStep()
{
    return timeStep;
}

void PhysicsSandbox::setTimeStep(float ts)
{
    timeStep = ts;
}

int PhysicsSandbox::getVelocityIterations()
{
    return velIter;
}

void PhysicsSandbox::setVelocityIterations(int velocityIterations)
{
    velIter = velocityIterations;
}

int PhysicsSandbox::getPositionIterations()
{
    return posIter;
}

void PhysicsSandbox::setPositionIterations(int positionIterations)
{
    posIter = positionIterations;
}

void PhysicsSandbox::setCameraPosition(float x, float y)
{
    cam.setCenter(x, y);
}

void PhysicsSandbox::setCameraZoom(float z)
{
    sf::Vector2u wSize = engine->Window.getSize();
    cam.setSize(wSize.x * z, wSize.y * z);
}

CScriptArray *PhysicsSandbox::getBodiesByName(std::string name)
{
    if (world)
    {
        vector<b2Body*> bodies;
        json.getBodiesByName(name, bodies);

        asITypeInfo* t = engine->ScriptEngine->GetTypeInfoByDecl("array<b2Body>");
        CScriptArray* arr = CScriptArray::Create(t, bodies.size());

        for (int i = 0; i < bodies.size(); i++)
            arr->SetValue(i, bodies[i]);

        return arr;
    }
    else
        return nullptr;
}

CScriptArray *PhysicsSandbox::getFixturesByName(std::string name)
{
    return nullptr;
}

CScriptArray *PhysicsSandbox::getJointsByName(std::string name)
{
    return nullptr;
}

void PhysicsSandbox::onPause()
{
    clock.stop();
}

void PhysicsSandbox::onResume()
{
    clock.start();
}
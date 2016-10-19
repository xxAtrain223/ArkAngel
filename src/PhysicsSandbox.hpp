//
// Created by Anthony on 5/27/2016.
//

#ifndef ARKANGEL_PHYSICSSANDBOX_HPP
#define ARKANGEL_PHYSICSSANDBOX_HPP

#include "Engine.hpp"
#include "Box2dDebugDraw.hpp"

#include <Box2D/Box2D.h>
#include <b2dJson.h>
#include <sfml/Graphics.hpp>
#include <Thor/Time.hpp>
#include <scriptarray.h>

class PhysicsSandbox
{
    Engine *engine = nullptr;
    const std::string SUID;

    sf::View cam;

    b2Vec2 getWorldPos(sf::Vector2i p);
    b2Vec2 getWorldPos(MousePosition p);

    b2World* world = nullptr;
    Box2dDebugDraw debugDraw;
    float timeStep;
    int velIter;
    int posIter;
    b2Body* groundBody;
    b2MouseJoint* mouseJoint = NULL;

    b2dJson json;

    thor::StopWatch clock;
    float accumulator;

    void loadB2World(std::string filename);

    float getTimeStep();
    void setTimeStep(float ts);
    int getVelocityIterations();
    void setVelocityIterations(int velocityIterations);
    int getPositionIterations();
    void setPositionIterations(int positionIterations);

    void setCameraPosition(float x, float y);
    void setCameraZoom(float z);

    CScriptArray* getBodiesByName(std::string name);
    CScriptArray* getFixturesByName(std::string name);
    CScriptArray* getJointsByName(std::string name);

    asITypeInfo* type;
    asIScriptModule* mod;
    asIScriptObject* obj;
    asIScriptContext* ctx;
    asIScriptFunction* updateFunc;

public:
    PhysicsSandbox() = default;
    PhysicsSandbox(std::string suid, Engine *engine);
    ~PhysicsSandbox();
    bool haltsHandleEvent() { return true; }
    bool haltsUpdate() { return true; }
    bool haltsDraw() { return true; }
    void handleEvent(sf::Event event) {}
    void update();
    void draw();
    const std::string getSUID() { return SUID; }
    void onPause();
    void onResume();
};


#endif //ARKANGEL_PHYSICSSANDBOX_HPP

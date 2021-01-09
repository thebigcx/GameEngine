#pragma once

#include <vector>

#include <physics/Body.h>

namespace Engine
{

class PhysicsWorld
{
public:
    PhysicsWorld();
    ~PhysicsWorld();

    void update(float delta);

    Body* createBody();

    void setGravity(const math::vec2& gravity) { m_gravitationalForce = gravity; }

private:
    std::vector<Body*> m_bodies;

    math::vec2 m_gravitationalForce;
};

}
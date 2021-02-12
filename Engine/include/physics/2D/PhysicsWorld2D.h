#pragma once

#include <vector>

#include <scene/GameComponent.h>

#include <box2d/box2d.h>

namespace Engine
{

class RigidBody2D;

class PhysicsWorld2D : public GameComponent
{
public:
    PhysicsWorld2D();
    ~PhysicsWorld2D();

    void onUpdate(float dt);

    void addRigidBody(RigidBody2D* body);
    void removeRigidBody(RigidBody2D* body);

    b2World& getWorld()
    {
        return m_world;
    }

private:
    b2World m_world;

    std::vector<RigidBody2D*> m_rigidBodies;
};

}
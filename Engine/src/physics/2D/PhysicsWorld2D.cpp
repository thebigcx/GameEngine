#include <physics/2D/PhysicsWorld2D.h>
#include <physics/2D/RigidBody2D.h>

#include <algorithm>

namespace Engine
{

PhysicsWorld2D::PhysicsWorld2D()
    : m_world(b2Vec2{ 0, 0 })
{
    
}

PhysicsWorld2D::~PhysicsWorld2D()
{

}

void PhysicsWorld2D::addRigidBody(RigidBody2D* body)
{
    m_rigidBodies.push_back(body);
}

void PhysicsWorld2D::removeRigidBody(RigidBody2D* body)
{
    auto it = std::find(m_rigidBodies.begin(), m_rigidBodies.end(), body);
    
    if (it != m_rigidBodies.end())
        m_rigidBodies.erase(it);
}

void PhysicsWorld2D::onUpdate(float dt)
{
    m_world.Step(dt, 8, 3);
}

}
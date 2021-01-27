#include <physics/2D/PhysicsController2D.h>

namespace Engine
{

PhysicsController2D::PhysicsController2D()
{
    
}

PhysicsController2D::~PhysicsController2D()
{

}

PhysicsController2D* PhysicsController2D::getInstance()
{
    static PhysicsController2D controller;
    return &controller;
}

void PhysicsController2D::onUpdate(float dt)
{

}

void PhysicsController2D::initialize()
{
    b2Vec2 gravity(0.f, -10.f);
    m_world = new b2World(gravity);
}

void PhysicsController2D::finalize()
{
    delete m_world;
}

}
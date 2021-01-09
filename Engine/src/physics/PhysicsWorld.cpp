#include <physics/PhysicsWorld.h>
#include <physics/BoxBody.h>
#include <maths/vector/vec_func.h>

#include <iostream>

namespace Engine
{

PhysicsWorld::PhysicsWorld()
{

}

PhysicsWorld::~PhysicsWorld()
{
    for (auto& body : m_bodies)
    {
        delete body;
    }
}

Body* PhysicsWorld::createBody()
{
    auto body = new BoxBody(); // TEMPORARY
    m_bodies.push_back(body);
    return body;
}

void PhysicsWorld::update(float delta)
{
    float dt = delta / 1000.f; // ms to s

    for (auto& body : m_bodies)
    {
        if (body->type != BodyType::Static)
        {
            math::vec2 acceleration = m_gravitationalForce * body->mass;

            body->position   += dt * body->m_velocity + acceleration * 0.5f * dt * dt;
            body->m_velocity += dt * acceleration;
        }
    }

    // Collisions

    for (unsigned int i = 0; i < m_bodies.size(); i++)
    for (unsigned int j = i + 1; j < m_bodies.size(); j++)
    {
        CollisionData collision = m_bodies[i]->m_collider->collide(*(m_bodies[j]->m_collider));

        if (collision.collision)
        {
            math::vec2 direction = math::normalize(collision.direction);
            math::vec2 otherDirection = math::reflect(direction, math::normalize(m_bodies[i]->m_velocity));

            m_bodies[i]->m_velocity = math::reflect(m_bodies[i]->m_velocity, direction);
            m_bodies[j]->m_velocity = math::reflect(m_bodies[j]->m_velocity, direction);

            std::cout << math::to_string(math::reflect(math::vec2(0, 1), math::vec2(0, -1))) << "\n";
        }
    }
}

}
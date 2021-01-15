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
        body->updateTransform();
    }

    for (auto& body : m_bodies)
    {
        if (body->type != BodyType::Static)
        {
            math::vec2 acceleration = m_gravitationalForce;

            body->position   += dt * body->m_velocity + acceleration * 0.5f * dt * dt;
            body->m_velocity += dt * acceleration;
        }
    }

    // Collisions //TODO: optimize division operations using inverse mass

    for (unsigned int a = 0; a < m_bodies.size(); a++)
    for (unsigned int b = a + 1; b < m_bodies.size(); b++)
    {
        CollisionData collision = m_bodies[a]->m_collider->collide(*(m_bodies[b]->m_collider));

        if (collision.collision)
        {
            float aMass = m_bodies[a]->mass;
            float bMass = m_bodies[b]->mass;

            math::vec2 normal = math::normalize(math::vec2(0, collision.direction.y));

            math::vec2 rv = m_bodies[b]->m_velocity - m_bodies[a]->m_velocity;
            float velNormal = math::dot(rv, normal);

            float e = math::min(m_bodies[a]->restitution, m_bodies[b]->restitution); // restitution
            
            float j = -(1 + e) * velNormal;
            j /= 1.f / aMass + 1.f / bMass;

            math::vec2 impulse = normal * j;
            
            if (m_bodies[a]->type != BodyType::Static)
                m_bodies[a]->m_velocity -= 1.f / aMass * impulse;

            if (m_bodies[b]->type != BodyType::Static)
                m_bodies[b]->m_velocity += 1.f / bMass * impulse;

            // Positional correct

            const float percent = 0.2f; // usually 20% to 80%
            const float slop = 0.01f;
            math::vec2 correction = (float)math::max(collision.getDistance() - slop, 0.f) / (1.f / aMass + 1.f / bMass) * percent * normal;
            if (m_bodies[a]->type != BodyType::Static)
                m_bodies[a]->position -= 1.f / aMass * correction;
            if (m_bodies[b]->type != BodyType::Static)
                m_bodies[b]->position += 1.f / bMass * correction;
        }
    }
}

}
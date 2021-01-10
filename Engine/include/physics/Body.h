#pragma once

#include <maths/math.h>

namespace Engine
{

class Collider;

enum class BodyType
{
    Static,
    Dynamic
};

class Body
{
public:

    virtual ~Body() = default;

    math::vec2 position;
    float mass = 0.f;
    float restitution = 2.f;
    BodyType type = BodyType::Dynamic;

protected:
    math::vec2 m_velocity;
    Collider* m_collider = nullptr;

    friend class PhysicsWorld;

};

}
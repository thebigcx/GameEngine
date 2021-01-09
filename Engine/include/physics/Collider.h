#pragma once

#include <physics/Body.h>

namespace Engine
{

class BoxBody;

struct CollisionData
{
    inline float getDistance() const { return math::length(direction); }
    
    bool collision;
    math::vec2 direction;
};

enum class ColliderType
{
    Box
};

class Collider
{
public:
    Collider(ColliderType type, const Body* owner)
        : m_type(type), m_owner(owner) {}

    virtual CollisionData collide(const Collider& other) = 0;
    
    virtual ~Collider() = default;

    ColliderType getType() const { return m_type; }

    const Body* getBody() const { return m_owner; }

protected:
    const Body* m_owner = nullptr;

private:
    ColliderType m_type;
};

class BoxCollider : public Collider
{
public:
    BoxCollider(const BoxBody* owner);

    CollisionData collide(const Collider& other) override;
};

}
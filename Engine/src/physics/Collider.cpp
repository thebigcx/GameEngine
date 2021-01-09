#include <physics/Collider.h>
#include <physics/BoxBody.h>

namespace Engine
{

BoxCollider::BoxCollider(const BoxBody* owner)
    : Collider(ColliderType::Box, static_cast<const Body*>(owner))
{

}

CollisionData BoxCollider::collide(const Collider& other)
{
    if (other.getType() == ColliderType::Box)
    {
        const BoxBody* body = static_cast<const BoxBody*>(m_owner);
        const BoxBody* otherBody = static_cast<const BoxBody*>(other.getBody());

        math::vec2 distances1 = otherBody->position - (body->position + body->size);
        math::vec2 distances2 = body->position - (otherBody->position + otherBody->size);
        math::vec2 distances = math::max(distances1, distances2);

        float maxDistance = math::max(distances);

        return { maxDistance < 0, distances };
    }
}

}
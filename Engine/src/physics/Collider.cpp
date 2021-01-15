#include <physics/Collider.h>
#include <physics/BoxBody.h>
#include <physics/PolygonBody.h>

#include <iostream>

namespace Engine
{

BoxCollider::BoxCollider(const BoxBody* owner)
    : Collider(ColliderType::Box, static_cast<const Body*>(owner))
{

}

PolygonCollider::PolygonCollider(const PolygonBody* owner)
    : Collider(ColliderType::Polygon, static_cast<const Body*>(owner))
{

}

CollisionData BoxCollider::collide(const Collider& other)
{
    const BoxBody* body = static_cast<const BoxBody*>(m_owner);

    if (other.getType() == ColliderType::Box)
    {
        const BoxBody* otherBody = static_cast<const BoxBody*>(other.getBody());

        math::vec2 distances1 = otherBody->position - (body->position + body->size);
        math::vec2 distances2 = body->position - (otherBody->position + otherBody->size);
        math::vec2 distances = math::max(distances1, distances2);

        float maxDistance = math::max(distances);

        return { maxDistance < 0, distances };
    }
}

CollisionData PolygonCollider::collide(const Collider& other)
{
    const PolygonBody* body = static_cast<const PolygonBody*>(m_owner);

    if (other.getType() == ColliderType::Polygon)
    {
        // SAT
        
        const PolygonBody* body2 = static_cast<const PolygonBody*>(other.getBody());

        for (unsigned int shape = 0; shape < 2; shape++)
        {
            if (shape == 1)
            {
                body = static_cast<const PolygonBody*>(other.getBody());
                body2 = static_cast<const PolygonBody*>(m_owner);
            }

            for (size_t a = 0; a < body->vertices.size(); a++)
            {
                int b = (a + 1) % body->vertices.size();
                math::vec2 axisProj = { -(body->vertices[b].y - body->vertices[a].y), (body->vertices[b].x - body->vertices[a].x) };

                float min_r2 = INFINITY, max_r2 = -INFINITY;

                for (size_t p = 0; p < body->vertices.size(); p++)
                {
                    float q = math::dot(body->vertices[p], axisProj);
                    min_r2 = math::min(min_r2, q);
                    max_r2 = math::max(max_r2, q);
                }

                if (!(max_r2 >= min_r2 && max_r2 >= min_r2))
                    return { false, 0 };
            }
        }

        std::cout << "polygon collision\n";
        return { true, 0 };
    }
}

}
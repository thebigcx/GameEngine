#pragma once

#include <vector>

#include <physics/Body.h>
#include <maths/math.h>

namespace Engine
{

class PolygonBody : public Body
{
public:
    void updateTransform() override;

    std::vector<math::vec2> vertices;

    float angle;

private:
    std::vector<math::vec2> m_transformedVertices;

    friend class PhysicsWorld;
};

}
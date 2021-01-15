#include <physics/PolygonBody.h>

namespace Engine
{

void PolygonBody::updateTransform()
{
    if (m_transformedVertices.size() != vertices.size())
        m_transformedVertices.resize(vertices.size());

    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        m_transformedVertices[i] =
        {
            (vertices[i].x * cosf(angle)) - (vertices[i].y * sinf(angle)) + position.x,
            (vertices[i].x * sinf(angle)) + (vertices[i].y * cosf(angle)) + position.y,
        };
    }
}

}
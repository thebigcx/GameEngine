#include "renderer/Quad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Quad::Quad()
{
    
}

Quad::Quad(const Vector2f& position, const Vector2f& size, const Color& color)
: m_color(color)
{
    m_position = position;
    m_size = size;
    m_texRect = FloatRect(0, 0, 1, 1);
}

std::array<Vertex, 4> Quad::getVertices() const
{
    // Transform the vertices
    auto transform = getTransform();
    
    std::array<Vertex, 4> vertices;

    for (int i = 0 ; i < 4 ; i++)
    {
        auto pos = transform * glm::vec4(m_positions[i].x, m_positions[i].y, 0, 1);
        vertices[i].position = Vector2f(pos.x, pos.y);

        vertices[i].color = m_color;
    }

    // TODO: find a way to incorporate this in for loop
    vertices[0].texCoord = m_texRect.getPosition();
    vertices[1].texCoord = Vector2f(m_texRect.x + m_texRect.width, m_texRect.y);
    vertices[2].texCoord = Vector2f(m_texRect.x + m_texRect.width, m_texRect.y + m_texRect.height);
    vertices[3].texCoord = Vector2f(m_texRect.x, m_texRect.y + m_texRect.height);
    
    return vertices;
}

void Quad::setTextureRect(const FloatRect& rect)
{
    m_texRect = rect;
}

void Quad::setColor(const Color& color)
{
    m_color = color;
}
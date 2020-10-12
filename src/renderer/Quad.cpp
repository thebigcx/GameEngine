#include "FGL/renderer/Quad.h"

Quad::Quad(const Vector2f& position, const Vector2f& size, const Color& color)
: m_position(position)
, m_color(color)
, m_size(size)
{

}

void Quad::move(Vector2f direction)
{
    m_position += direction;
}

std::array<Vertex, 4> Quad::getVertices() const
{
    Vertex a;
    a.position = m_position;
    a.color = m_color;

    Vertex b;
    b.position = Vector2f(m_position.x + m_size.x, m_position.y);
    b.color = m_color;

    Vertex c;
    c.position = Vector2f(m_position.x + m_size.x, m_position.y + m_size.y);
    c.color = m_color;

    Vertex d;
    d.position = Vector2f(m_position.x, m_position.y + m_size.y);
    d.color = m_color;

    return { a, b, c, d };
}
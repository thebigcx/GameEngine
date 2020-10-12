#include "FGL/renderer/Quad.h"

Quad::Quad(const Vector2f& position, const Vector2f& size, const Color& color)
: m_position(position)
, m_color(color)
, m_size(size)
{
    m_positions[0] = position;
    m_positions[1] = Vector2f(position.x + size.x, position.y);
    m_positions[2] = Vector2f(position.x + size.x, position.y + size.y);
    m_positions[3] = Vector2f(position.x, position.y + size.y);

    m_colors[0] = color;
    m_colors[1] = color;
    m_colors[2] = color;
    m_colors[3] = color;
}

std::array<Vertex, 4> Quad::getVertices()
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
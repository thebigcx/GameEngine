#include "renderer/Quad.h"

Quad::Quad(const Vector2f& position, const Vector2f& size, const Color& color)
: m_position(position)
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
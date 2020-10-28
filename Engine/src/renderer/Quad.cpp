#include <renderer/Quad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Quad::Quad()
{
    
}

Quad::Quad(const Vector2f& position, const Vector2f& size)
: m_color(Color(1, 1, 1, 1))
{
    m_position = position;
    m_size = size;
}

Quad::Quad(const Vector2f& position, const Vector2f& size, const Color& color)
: m_color(color)
{
    m_position = position;
    m_size = size;
}

Quad::Quad(const FloatRect& rect)
: m_color(Color(1, 1, 1, 1))
{
    m_position = rect.getPosition();
    m_size = rect.getSize();
}

Quad::Quad(float x, float y, float width, float height)
{
    m_position = Vector2f(x, y);
    m_size = Vector2f(width, height);
}

void Quad::setTextureRect(const FloatRect& rect)
{
    m_texRect = rect;
}

void Quad::setColor(const Color& color)
{
    m_color = color;
}
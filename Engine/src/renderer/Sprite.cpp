#include <renderer/Sprite.h>

Sprite::Sprite()
{
    
}

Sprite::Sprite(const Vector2f& position, const Vector2f& size)
: m_color(Color(1, 1, 1, 1))
{
    m_position = position;
    m_size = size;
}

Sprite::Sprite(const Vector2f& position, const Vector2f& size, const Color& color)
: m_color(color)
{
    m_position = position;
    m_size = size;
}

Sprite::Sprite(const FloatRect& rect)
: m_color(Color(1, 1, 1, 1))
{
    m_position = rect.getPosition();
    m_size = rect.getSize();
}

Sprite::Sprite(float x, float y, float width, float height)
{
    m_position = Vector2f(x, y);
    m_size = Vector2f(width, height);
}

void Sprite::setTextureRect(const FloatRect& rect)
{
    m_texRect = rect;
}

void Sprite::setColor(const Color& color)
{
    m_color = color;
}
#include <renderer/Sprite.h>

Sprite::Sprite()
{
    
}

Sprite::Sprite(const math::Vector2f& position, const math::Vector2f& size)
: m_color(math::Vector4f(1, 1, 1, 1))
{
    m_position = position;
    m_size = size;
}

Sprite::Sprite(const math::Vector2f& position, const math::Vector2f& size, const math::Vector4f& color)
: m_color(color)
{
    m_position = position;
    m_size = size;
}

Sprite::Sprite(const FloatRect& rect)
: m_color(math::Vector4f(1, 1, 1, 1))
{
    m_position = rect.getPosition();
    m_size = rect.getSize();
}

Sprite::Sprite(float x, float y, float width, float height)
{
    m_position = math::Vector2f(x, y);
    m_size = math::Vector2f(width, height);
}

void Sprite::setTextureRect(const FloatRect& rect)
{
    m_texRect = rect;
}

void Sprite::setColor(const math::Vector4f& color)
{
    m_color = color;
}
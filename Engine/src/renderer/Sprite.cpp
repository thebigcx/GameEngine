#include <renderer/Sprite.h>

Sprite::Sprite()
{
    
}

Sprite::Sprite(const math::vec2& position, const math::vec2& size)
: m_color(math::vec4(1, 1, 1, 1))
{
    m_position = position;
    m_size = size;
}

Sprite::Sprite(const math::vec2& position, const math::vec2& size, const math::vec4& color)
: m_color(color)
{
    m_position = position;
    m_size = size;
}

Sprite::Sprite(const FloatRect& rect)
: m_color(math::vec4(1, 1, 1, 1))
{
    m_position = rect.getPosition();
    m_size = rect.getSize();
}

Sprite::Sprite(float x, float y, float width, float height)
{
    m_position = math::vec2(x, y);
    m_size = math::vec2(width, height);
}

void Sprite::setTextureRect(const FloatRect& rect)
{
    m_texRect = rect;
}

void Sprite::setColor(const math::vec4& color)
{
    m_color = color;
}
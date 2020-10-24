#pragma once

class UIComponent
{
public:
    UIComponent() {}

    inline void setPosition(const Vector2f& position)
    {
        m_position = position;
    }

    inline void setSize(const Vector2f& size)
    {
        m_size = size;
    }

    inline const Vector2f& getPosition() const { return m_position; }
    inline const Vector2f& getSize() const { return m_size; }

private:
    Vector2f m_position;
    Vector2f m_size;
};
#pragma once

#include <events/Event.h>

namespace Engine
{

class MouseButtonEvent : public Event
{
public:
    inline int32_t getButton() const
    {
        return m_button;
    }

    inline float getX() const
    {
        return m_position.x;
    }

    inline float getY() const
    {
        return m_position.y;
    }

    inline const math::vec2& getPosition() const
    {
        return m_position;
    }

    virtual Event::Type type() const override = 0;
    EVENT_CLASS_CATEGORY(Event::Category::MouseButton)

protected:
    MouseButtonEvent(int32_t button, float x, float y)
        : m_button(button), m_position(x, y) {}

    int32_t m_button;
    math::vec2 m_position;
};

class MousePressedEvent : public MouseButtonEvent
{
public:
    MousePressedEvent(int32_t button, float x, float y)
        : MouseButtonEvent(button, x, y) {}

    EVENT_CLASS_TYPE(MousePressed);
};

class MouseReleasedEvent : public MouseButtonEvent
{
public:
    MouseReleasedEvent(int32_t button, float x, float y)
        : MouseButtonEvent(button, x, y) {}

    EVENT_CLASS_TYPE(MouseReleased);
};

class MouseMovedEvent : public Event
{
public:
    MouseMovedEvent(float x, float y, bool dragged)
        : m_dragged(dragged), m_position(x, y) {}

    inline float getX() const
    {
        return m_position.x;
    }

    inline float getY() const
    {
        return m_position.y;
    }

    inline const math::vec2& getPosition() const
    {
        return m_position;
    }

    EVENT_CLASS_TYPE(MouseMoved);
    EVENT_CLASS_CATEGORY(Event::Category::Mouse);

private:
    bool m_dragged;
    math::vec2 m_position;
};

class MouseScrollEvent : public Event
{
public:
    MouseScrollEvent(float xoffset, float yoffset)
        : m_offset(xoffset, yoffset) {}

    inline int getXOffset() const
    {
        return m_offset.x;
    }

    inline int getYOffset() const
    {
        return m_offset.y;
    }

    inline const math::ivec2& getOffset() const
    {
        return m_offset;
    }

    EVENT_CLASS_TYPE(MouseScroll);
    EVENT_CLASS_CATEGORY(Event::Category::Mouse);

private:
    math::ivec2 m_offset;
};

class MouseEnterEvent : public Event
{
public:
    MouseEnterEvent() = default;

    EVENT_CLASS_TYPE(MouseEnter);
    EVENT_CLASS_CATEGORY(Event::Category::Mouse);
};

class MouseLeaveEvent : public Event
{
public:
    MouseLeaveEvent() = default;

    EVENT_CLASS_TYPE(MouseLeave);
    EVENT_CLASS_CATEGORY(Event::Category::Mouse);
};

}
#pragma once

#include <events/Event.h>

namespace Engine
{

class WindowResizeEvent : public Event
{
public:
    WindowResizeEvent(uint32_t width, uint32_t height)
        : m_size(width, height) {}

    inline uint32_t getWidth() const
    {
        return m_size.x;
    }

    inline uint32_t getHeight() const
    {
        return m_size.y;
    }

    EVENT_CLASS_TYPE(WindowResize);
    EVENT_CLASS_CATEGORY(Event::Category::Window)

protected:
    math::uvec2 m_size;
};

class WindowCloseEvent : public Event
{
public:
    WindowCloseEvent() = default;

    EVENT_CLASS_TYPE(WindowClose);
    EVENT_CLASS_CATEGORY(Event::Category::Window)
};

class WindowPositionEvent : public Event
{
public:
    WindowPositionEvent(int x, int y)
        : m_pos(x, y) {}

    int getX() const
    {
        return m_pos.x;
    }

    int getY() const
    {
        return m_pos.y;
    }

    const math::ivec2& getPosition() const
    {
        return m_pos;
    }

    EVENT_CLASS_TYPE(WindowPosition);
    EVENT_CLASS_CATEGORY(Event::Category::Window)

private:
    math::ivec2 m_pos;
};

class WindowMaximizeEvent : public Event
{
public:
    WindowMaximizeEvent() = default;

    EVENT_CLASS_TYPE(WindowMaximize);
    EVENT_CLASS_CATEGORY(Event::Category::Window)
};

class WindowUnmaximizeEvent : public Event
{
public:
    WindowUnmaximizeEvent() = default;

    EVENT_CLASS_TYPE(WindowUnmaximize);
    EVENT_CLASS_CATEGORY(Event::Category::Window)
};

class WindowMinimizeEvent : public Event
{
public:
    WindowMinimizeEvent() = default;

    EVENT_CLASS_TYPE(WindowMinimize);
    EVENT_CLASS_CATEGORY(Event::Category::Window)
};

class WindowUnminimizeEvent : public Event
{
public:
    WindowUnminimizeEvent() = default;

    EVENT_CLASS_TYPE(WindowUnminimize);
    EVENT_CLASS_CATEGORY(Event::Category::Window)
};

class WindowRefreshEvent : public Event
{
public:
    WindowRefreshEvent() = default;

    EVENT_CLASS_TYPE(WindowRefresh);
    EVENT_CLASS_CATEGORY(Event::Category::Window)
};

class WindowFocusEvent : public Event
{
public:
    WindowFocusEvent() = default;

    EVENT_CLASS_TYPE(WindowFocus);
    EVENT_CLASS_CATEGORY(Event::Category::Window)
};

class WindowUnfocusEvent : public Event
{
public:
    WindowUnfocusEvent() = default;

    EVENT_CLASS_TYPE(WindowUnfocus);
    EVENT_CLASS_CATEGORY(Event::Category::Window)
};

}
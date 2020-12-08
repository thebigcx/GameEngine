#pragma once

#include <cstdint>

#include <maths/math.h>
#include <core/Input.h>

enum class EventType
{
    KeyPressed, KeyReleased, KeyTyped, 
    MousePressed, MouseReleased, MouseMoved, MouseScroll,
    MouseEnter, MouseLeave,
    WindowResize, WindowClose, 
    WindowMaximize, WindowUnmaximize, 
    WindowMinimize, WindowUnminimize,
    WindowPosition, WindowRefresh,
    WindowFocus, WindowUnfocus
};

enum class EventCategory
{
    Keyboard    = 1 << 0, 
    Mouse       = 1 << 1, 
    MouseButton = 1 << 2, 
    Window      = 1 << 3
};

#define EVENT_CLASS_TYPE(evtype) static EventType staticType() { return EventType::evtype; } \
                                 inline EventType type() const override { return staticType(); }

//#define EVENT_CLASS_CATEGORY(...) inline uint32_t categories() const override { return getEnumCateogry(VA_ARGS) }
#define EVENT_CLASS_CATEGORY(category) inline uint32_t categories() const override { return (uint32_t)category; }

template<typename... Args>
uint32_t getEnumCategory(Args... args)
{
    uint32_t value;
    std::vector<uint32_t> vals(args...);
    for (unsigned int i = 0; i < vals.size(); i++)
    {
        value |= (uint32_t)vals[i];
    }
    return value;
}

class Event
{
public:
    virtual ~Event() = default;

    virtual EventType type() const = 0;
    virtual uint32_t categories() const = 0;

    bool inCategory(EventCategory category) const
    {
        return static_cast<bool>(categories() & static_cast<uint32_t>(category));
    }

    bool handled = false;

protected:
    
};

struct IsEvent
{
    static constexpr bool detect(Event* event)
    {
        return std::is_base_of<Event, decltype(event)>::value;
    }

    template<typename T>
    static constexpr bool detect()
    {
        return std::is_base_of<Event, T>::value;
    }

};

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
    EVENT_CLASS_CATEGORY(EventCategory::Window)

protected:
    math::uvec2 m_size;
};

class WindowCloseEvent : public Event
{
public:
    WindowCloseEvent() = default;

    EVENT_CLASS_TYPE(WindowClose);
    EVENT_CLASS_CATEGORY(EventCategory::Window)
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
    EVENT_CLASS_CATEGORY(EventCategory::Window)

private:
    math::ivec2 m_pos;
};

class WindowMaximizeEvent : public Event
{
public:
    WindowMaximizeEvent() = default;

    EVENT_CLASS_TYPE(WindowMaximize);
    EVENT_CLASS_CATEGORY(EventCategory::Window)
};

class WindowUnmaximizeEvent : public Event
{
public:
    WindowUnmaximizeEvent() = default;

    EVENT_CLASS_TYPE(WindowUnmaximize);
    EVENT_CLASS_CATEGORY(EventCategory::Window)
};

class WindowMinimizeEvent : public Event
{
public:
    WindowMinimizeEvent() = default;

    EVENT_CLASS_TYPE(WindowMinimize);
    EVENT_CLASS_CATEGORY(EventCategory::Window)
};

class WindowUnminimizeEvent : public Event
{
public:
    WindowUnminimizeEvent() = default;

    EVENT_CLASS_TYPE(WindowUnminimize);
    EVENT_CLASS_CATEGORY(EventCategory::Window)
};

class WindowRefreshEvent : public Event
{
public:
    WindowRefreshEvent() = default;

    EVENT_CLASS_TYPE(WindowRefresh);
    EVENT_CLASS_CATEGORY(EventCategory::Window)
};

class WindowFocusEvent : public Event
{
public:
    WindowFocusEvent() = default;

    EVENT_CLASS_TYPE(WindowFocus);
    EVENT_CLASS_CATEGORY(EventCategory::Window)
};

class WindowUnfocusEvent : public Event
{
public:
    WindowUnfocusEvent() = default;

    EVENT_CLASS_TYPE(WindowUnfocus);
    EVENT_CLASS_CATEGORY(EventCategory::Window)
};

class KeyEvent : public Event
{
public:
    KeyEvent(int32_t keyCode, EventType type)
        : m_keyCode(keyCode) {}

    inline int32_t getKeyCode() const
    {
        return m_keyCode;
    }

    virtual EventType type() const override = 0;
    EVENT_CLASS_CATEGORY(EventCategory::Keyboard)

protected:
    int32_t m_keyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
    KeyPressedEvent(int32_t button, int32_t repeat, int32_t mods)
        : KeyEvent(button, EventType::KeyPressed), m_repeat(repeat), m_mods(mods) {}

    inline int32_t getRepeat() const
    {
        return m_repeat;
    }

    inline int32_t getMods() const
    {
        return m_mods;
    }

    inline bool isModifier(int32_t mod) const
    {
        return static_cast<bool>(m_mods & mod);
    }

    EVENT_CLASS_TYPE(KeyPressed);

private:
    int32_t m_repeat, m_mods;
};

class KeyReleasedEvent : public KeyEvent
{
public:
    KeyReleasedEvent(int32_t button)
        : KeyEvent(button, EventType::KeyReleased) {}

    EVENT_CLASS_TYPE(KeyReleased);
};

class KeyTypedEvent : public KeyEvent
{
public:
    KeyTypedEvent(Key keycode);

    EVENT_CLASS_TYPE(KeyTyped);
};

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

    virtual EventType type() const override = 0;
    EVENT_CLASS_CATEGORY(EventCategory::MouseButton)

protected:
    MouseButtonEvent(int32_t button, float x, float y, EventType type)
        : m_button(button), m_position(x, y) {}

    int32_t m_button;
    math::vec2 m_position;
};

class MousePressedEvent : public MouseButtonEvent
{
public:
    MousePressedEvent(int32_t button, float x, float y)
        : MouseButtonEvent(button, x, y, EventType::MousePressed) {}

    EVENT_CLASS_TYPE(MousePressed);
};

class MouseReleasedEvent : public MouseButtonEvent
{
public:
    MouseReleasedEvent(int32_t button, float x, float y)
        : MouseButtonEvent(button, x, y, EventType::MouseReleased) {}

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
    EVENT_CLASS_CATEGORY(EventCategory::Mouse);

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
    EVENT_CLASS_CATEGORY(EventCategory::Mouse);

private:
    math::ivec2 m_offset;
};

class MouseEnterEvent : public Event
{
public:
    MouseEnterEvent() = default;

    EVENT_CLASS_TYPE(MouseEnter);
    EVENT_CLASS_CATEGORY(EventCategory::Mouse);
};

class MouseLeaveEvent : public Event
{
public:
    MouseLeaveEvent() = default;

    EVENT_CLASS_TYPE(MouseLeave);
    EVENT_CLASS_CATEGORY(EventCategory::Mouse);
};
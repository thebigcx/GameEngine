#pragma once

#include <cstdint>

#include <maths/math.h>

enum class EventType
{
    KeyPressed, KeyReleased, MousePressed, MouseMoved, MouseReleased, WindowResized
};

class Event
{
public:
    virtual ~Event() = default;

    virtual EventType getEventType() const = 0;

protected:
};

class WindowResizedEvent : public Event
{
public:
    WindowResizedEvent(uint32_t width, uint32_t height)
        : m_size(width, height) {}

    inline uint32_t getWidth() const
    {
        return m_size.x;
    }

    inline uint32_t getHeight() const
    {
        return m_size.y;
    }

    static EventType getStaticType() { return EventType::WindowResized; }
    inline EventType getEventType() const override { return getStaticType(); }

protected:
    math::uvec2 m_size;
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

    virtual EventType getEventType() const override = 0;

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

    inline EventType getEventType() const override { return getStaticType(); }
    static EventType getStaticType() { return EventType::KeyPressed; }

private:
    int32_t m_repeat, m_mods;
};

class KeyReleasedEvent : public KeyEvent
{
public:
    KeyReleasedEvent(int32_t button)
        : KeyEvent(button, EventType::KeyReleased) {}

    inline EventType getEventType() const override { return getStaticType(); }
    static EventType getStaticType() { return EventType::KeyReleased; }
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

    virtual EventType getEventType() const override = 0;

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

    static EventType getStaticType() { return EventType::MousePressed; }
    inline EventType getEventType() const override { return getStaticType(); }
};

class MouseReleasedEvent : public MouseButtonEvent
{
public:
    MouseReleasedEvent(int32_t button, float x, float y)
        : MouseButtonEvent(button, x, y, EventType::MouseReleased) {}

    static EventType getStaticType() { return EventType::MouseReleased; }
    inline EventType getEventType() const override { return getStaticType(); }
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

    static EventType getStaticType() { return EventType::MouseMoved; }
    inline EventType getEventType() const override { return getStaticType(); }

private:
    bool m_dragged;
    math::vec2 m_position;
};
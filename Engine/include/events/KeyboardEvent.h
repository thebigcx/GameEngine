#pragma once

#include <events/Event.h>
#include <core/Keyboard.h>

namespace Engine
{

class KeyEvent : public Event
{
public:
    KeyEvent(int32_t keyCode)
        : m_keyCode(keyCode) {}

    inline int32_t getKeyCode() const
    {
        return m_keyCode;
    }

    virtual Event::Type type() const override = 0;
    EVENT_CLASS_CATEGORY(Event::Category::Keyboard)

protected:
    int32_t m_keyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
    KeyPressedEvent(int32_t button, int32_t repeat, int32_t mods)
        : KeyEvent(button), m_repeat(repeat), m_mods(mods) {}

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
        : KeyEvent(button) {}

    EVENT_CLASS_TYPE(KeyReleased);
};

class KeyTypedEvent : public KeyEvent
{
public:
    KeyTypedEvent(Keyboard::Key keycode);

    EVENT_CLASS_TYPE(KeyTyped);
};

}
#pragma once

#include <events/Event.h>
#include <core/Gamepad.h>

namespace Engine
{

class GamepadEvent : public Event
{
public:
    GamepadEvent(Gamepad::ID id)
        : m_gamepadId(id) {}

    inline Gamepad::ID getGamepadId() const
    {
        return m_gamepadId;
    }

    EVENT_CLASS_CATEGORY(Event::Category::Gamepad);

protected:
    Gamepad::ID m_gamepadId;
};

class GamepadConnectEvent : public GamepadEvent
{
public:
    GamepadConnectEvent(Gamepad::ID id)
        : GamepadEvent(id) {}

    EVENT_CLASS_TYPE(GamepadConnect);
};

class GamepadDisconnectEvent : public GamepadEvent
{
public:
    GamepadDisconnectEvent(Gamepad::ID id)
        : GamepadEvent(id) {}

    EVENT_CLASS_TYPE(GamepadDisconnect);
};

}
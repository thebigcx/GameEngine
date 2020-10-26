#pragma once

union EventData
{
    struct window
    {
        int width;
        int height;
    } window;

    struct key
    {
        int keycode;
        int scancode;
        int action;
        int mods;
    } key;

    struct typekey
    {
        unsigned int code;
    } typekey;

    struct mousepos
    {
        double x;
        double y;
    } mousepos;

    struct mouseclick
    { 
        int button;
        bool action;
        int mods;
    } mouseclick;
};

enum EventType
{
    None = 0,
    WindowClose, WindowResize, KeyPressed, KeyReleased, KeyTyped,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

class Event
{
public:
    Event()
        : m_type(EventType::None) {}
    Event(EventType type, EventData data);

    inline EventData& data()
    {
        return m_data;
    }

    inline EventData data() const
    {
        return m_data;
    }

    inline EventType type() const
    {
        return m_type;
    }

private:
    EventType m_type;
    EventData m_data;
};
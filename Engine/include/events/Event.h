#pragma once

#include <cstdint>

#include <maths/math.h>

namespace Engine
{

#define EVENT_CLASS_TYPE(evtype) static Event::Type staticType() { return Event::Type::evtype; } \
                                 inline Event::Type type() const override { return staticType(); }

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
    enum class Type
    {
        KeyPressed, KeyReleased, KeyTyped, 
        MousePressed, MouseReleased, MouseMoved, MouseScroll,
        MouseEnter, MouseLeave,
        WindowResize, WindowClose, 
        WindowMaximize, WindowUnmaximize, 
        WindowMinimize, WindowUnminimize,
        WindowPosition, WindowRefresh,
        WindowFocus, WindowUnfocus,
        GamepadConnect, GamepadDisconnect
    };

    enum class Category
    {
        Keyboard    = 1 << 0, 
        Mouse       = 1 << 1, 
        MouseButton = 1 << 2, 
        Window      = 1 << 3,
        Gamepad     = 1 << 4
    };

public:
    virtual ~Event() = default;

    virtual Event::Type type() const = 0;
    virtual uint32_t categories() const = 0;

    bool inCategory(Event::Category category) const
    {
        return static_cast<bool>(categories() & static_cast<uint32_t>(category));
    }

    bool handled = false;

protected:
    
};

template<typename T>
struct IsEvent
{
    static constexpr bool value = std::is_base_of<Event, T>::value;
};

}
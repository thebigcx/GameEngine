#pragma once

#include <functional>

#include <events/Event.h>

class EventManager
{
public:
    static void setupCallbacks();
};

class EventDispatcher
{
public:
    EventDispatcher(Event& event)
        : m_event(event) {}

    template<typename T, typename F>
    void dispatch(const F& func)
    {
        if (T::staticType() == m_event.type())
        {
            func(static_cast<T&>(m_event));
        }
    }

private:
    Event& m_event;
};

#define BIND_EVENT_FN(fn) [this](auto&& ...args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#pragma once

#include <events/Event.h>
#include <core/Logger.h>

namespace Engine
{

class EventDispatcher
{
public:
    EventDispatcher(Event& event)
        : m_event(event) {}

    template<typename T, typename F>
    void dispatch(const F& func)
    {
        static_assert(IsEvent::detect<T>(), "Not a derived class of Event.");
        
        if (T::staticType() == m_event.type())
        {
            func(static_cast<T&>(m_event));
        }
    }

private:
    Event& m_event;
};

#define BIND_EVENT_FN(fn) [this](auto&& ...args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

}
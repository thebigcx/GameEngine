#pragma once

#include <vector>

#include "Event.h"

class EventStack
{
public:
    EventStack() {}

    void push(const Event& event)
    {
        m_events.push_back(event);
    }

    void pop()
    {
        m_events.pop_back();
    }

    Event getEvent()
    {
        return m_events[m_events.size() - 1];
    }

    bool isEmpty()
    {
        return m_events.size() == 0;
    }

private:
    std::vector<Event> m_events;
};
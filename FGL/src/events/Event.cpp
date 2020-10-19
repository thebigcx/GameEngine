#include <events/Event.h>

Event::Event(EventType type, EventData data)
    : m_type(type), m_data(data)
{

}
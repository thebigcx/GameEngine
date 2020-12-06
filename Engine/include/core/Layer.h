#pragma once

#include <events/Event.h>

class Layer
{
public:
    Layer() {};
    virtual ~Layer() = default;

    virtual void onAttach() {}
    virtual void onUpdate() {}
    virtual void onEvent(Event& event) {}

protected:
    
};
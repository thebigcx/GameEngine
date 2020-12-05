#pragma once

#include <events/Event.h>

class Layer
{
public:
    Layer() {};
    virtual ~Layer() = default;

    virtual void update() {};
    virtual void handleEvent(Event& event) {};

protected:
    
};
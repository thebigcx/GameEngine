#pragma once

#include <events/Event.h>

class Layer
{
public:
    Layer() {};
    virtual ~Layer() = default;

    virtual void update() = 0;
    virtual void handleEvent(const Event& event) = 0;

protected:
    
};
#pragma once

#include <events/Event.h>

class Layer
{
public:
    Layer() {};
    virtual ~Layer() = default;

    virtual void update(float dt) = 0;
    virtual void handleEvent(const Event& event) = 0;

protected:
    
};
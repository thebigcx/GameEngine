#pragma once

class Layer
{
public:
    Layer();
    virtual ~Layer() = default;

    virtual void update() = 0;

protected:
    
};
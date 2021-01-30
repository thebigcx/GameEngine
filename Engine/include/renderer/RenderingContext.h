#pragma once

#include <core/Core.h>

namespace Engine
{

class RenderingContext
{
public:
    virtual ~RenderingContext() = default;
    
    virtual void init() = 0;
    virtual void swapBuffers() = 0;
    virtual void vsync(bool enabled) = 0;

    static Owned<RenderingContext> create(void* window);
};

}
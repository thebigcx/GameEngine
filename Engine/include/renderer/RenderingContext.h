#pragma once

#include <core/Core.h>
#include <GLFW/glfw3.h>

class RenderingContext
{
public:
    virtual ~RenderingContext() = default;
    
    virtual void init() = 0;
    virtual void swapBuffers() = 0;
    virtual void vsync(bool enabled) = 0;

    static Unique<RenderingContext> create(GLFWwindow* window);
};
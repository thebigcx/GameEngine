#pragma once

#include <core/Core.h>
#include <GLFW/glfw3.h>

class GraphicsContext
{
public:
    virtual void init() = 0;
    virtual void swapBuffers() = 0;

    static Unique<GraphicsContext> create(GLFWwindow* window);
};
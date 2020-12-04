#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <renderer/RenderingContext.h>

class GLContext : public RenderingContext
{
public:
    GLContext(GLFWwindow* window);

    void init() override;
    void swapBuffers() override;
    void vsync(bool enabled) override;

private:
    GLFWwindow* m_window;
};
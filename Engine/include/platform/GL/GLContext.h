#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <renderer/GraphicsContext.h>

class GLContext : public GraphicsContext
{
public:
    GLContext(GLFWwindow* window);

    void init() override;
    void swapBuffers() override;

private:
    GLFWwindow* m_window;
};
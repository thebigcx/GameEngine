#include <platform/GL/GLContext.h>

GLContext::GLContext(GLFWwindow* window)
    : m_window(window)
{

}

void GLContext::init()
{
    glfwMakeContextCurrent(m_window);

    glewInit();
}

void GLContext::swapBuffers()
{
    glfwSwapBuffers(m_window);
}

void GLContext::vsync(bool enabled)
{
    int interval = enabled ? 1 : 0;
    glfwSwapInterval(interval);
}
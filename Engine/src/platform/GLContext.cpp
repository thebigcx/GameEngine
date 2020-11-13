#include <platform/GLContext.h>

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
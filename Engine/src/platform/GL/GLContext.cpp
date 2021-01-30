#include <platform/GL/GLContext.h>
#include <core/Logger.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Engine
{

GLContext::GLContext(GLFWwindow* window)
    : m_window(window)
{

}

static void errorCallbackGLFW(int errorCode, const char* description)
{
    Logger::getCoreLogger()->error("[GLFW] Error code: %d. %s", errorCode, description);
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

}
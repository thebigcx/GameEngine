#include "Window.h"

Window::Window(int width, int height, std::string title)
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (m_window == nullptr)
    {
        printf("Failed to create GLFW window.\n");
        glfwTerminate();
    }

    glfwSetWindowSizeLimits(m_window, 200, 200, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

    glfwSwapInterval(1);

    glViewport(0, 0, 1920, 1080);

    glfwMakeContextCurrent(m_window);

    glewInit();
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Window::setActive()
{
    glfwMakeContextCurrent(m_window);
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

bool Window::isOpen()
{
    return !glfwWindowShouldClose(m_window);
}

void Window::clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

bool Window::isKeyPressed(int key)
{
    return glfwGetKey(m_window, key);
}

void Window::close()
{
    glfwSetWindowShouldClose(m_window, true);
}

void Window::display()
{
    glfwSwapBuffers(m_window);
}

void Window::pollEvents()
{
    glfwPollEvents();
}
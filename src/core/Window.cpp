#include "FGL/core/Window.h"
#include "FGL/core/Mouse.h"

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
}

void Window::setActive()
{
    glfwMakeContextCurrent(m_window);
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

Vector2i Window::getSize() const
{
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    return Vector2i(width, height);
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

void Window::display() const
{
    glfwSwapBuffers(m_window);
}

void Window::pollEvents()
{
    glfwWaitEvents();
}

void Window::setIcon(const std::string& iconPath)
{
    Image image;
    image.loadFile(iconPath);
    setIcon(image);
}

void Window::setIcon(const Image& image)
{
    GLFWimage icon;
    icon.pixels = image.getPixels();
    icon.width = image.getSize().x;
    icon.height = image.getSize().y;
    glfwSetWindowIcon(m_window, 1, &icon);
}

void Window::setTitle(const std::string& title)
{
    glfwSetWindowTitle(m_window, title.c_str());
}

void Window::setSize(const Vector2u& size)
{
    glfwSetWindowSize(m_window, size.x, size.y);
}

void Window::minimize()
{
    glfwIconifyWindow(m_window);
}

void Window::maximize()
{
    glfwMaximizeWindow(m_window);
}

void Window::restore()
{
    glfwRestoreWindow(m_window);
}

void Window::setOpacity(float opacity)
{
    glfwSetWindowOpacity(m_window, opacity);
}

void Window::makeCurrentContext() const
{
    glfwMakeContextCurrent(m_window);
}
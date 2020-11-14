#include <core/Window.h>

#include <iostream>

static void GLFWErrorCallback(int error, const char* description)
{
    std::cout << "GLFW error (" << error << "): " << description << "\n";
}

Window::Window(int width, int height, const std::string& title)
{
    init(width, height, title);
}

void Window::init(int width, int height, const std::string& title)
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwSetErrorCallback(GLFWErrorCallback);

    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (m_window == nullptr)
    {
        std::cout << "Failed to create GLFW window.\n";
        glfwTerminate();
    }

    glfwSetWindowSizeLimits(m_window, 200, 200, GLFW_DONT_CARE, GLFW_DONT_CARE);

    glViewport(0, 0, 1920, 1080);

    m_context = GraphicsContext::create(m_window);
    m_context->init();

    glfwSwapInterval(1);
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
}

Unique<Window> Window::create(int width, int height, const std::string& title)
{
    return createUnique<Window>(width, height, title);
}

math::Vector2i Window::getSize() const
{
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    return math::Vector2i(width, height);
}

void Window::onUpdate()
{
    m_context->swapBuffers();
}

bool Window::isOpen()
{
    return !glfwWindowShouldClose(m_window);
}

void Window::close()
{
    glfwSetWindowShouldClose(m_window, true);
}

void Window::pollEvents()
{
    glfwPollEvents();
}

void Window::setIcon(const std::string& iconPath)
{
    auto image = ImageLoader::loadImage(iconPath);
    setIcon(*image);
}

void Window::setIcon(const Image& image)
{
    GLFWimage icon;
    icon.pixels = image.data;
    icon.width = image.width;
    icon.height = image.height;
    glfwSetWindowIcon(m_window, 1, &icon);
}

void Window::setTitle(const std::string& title)
{
    glfwSetWindowTitle(m_window, title.c_str());
}

void Window::setSize(const math::Vector2u& size)
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
#include <core/Window.h>
#include <core/WindowEventCallbacks.h>

#include <iostream>

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

    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (m_window == nullptr)
    {
        std::cout << "Failed to create GLFW window.\n";
        glfwTerminate();
    }

    glfwSetWindowSizeLimits(m_window, 200, 200, GLFW_DONT_CARE, GLFW_DONT_CARE);

    m_context = RenderingContext::create(m_window);
    m_context->init();
    m_context->vsync(true);

    glfwSetWindowUserPointer(m_window, &m_data);

    glfwSetWindowCloseCallback(m_window, windowCloseCallback);
    glfwSetWindowMaximizeCallback(m_window, windowMaximizeCallback);
    glfwSetWindowPosCallback(m_window, windowPosCallback);
    glfwSetWindowRefreshCallback(m_window, windowRefreshCallback);
    glfwSetWindowFocusCallback(m_window, windowFocusCallback);
    glfwSetWindowIconifyCallback(m_window, windowIconifyCallback);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
    glfwSetKeyCallback(m_window, keyCallback);
    glfwSetCharCallback(m_window, characterCallback);
    glfwSetCursorPosCallback(m_window, cursorPositionCallback);
    glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
    glfwSetScrollCallback(m_window, mouseScrollCallback);
    glfwSetCursorEnterCallback(m_window, mouseEnterCallback);
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
}

Unique<Window> Window::create(int width, int height, const std::string& title)
{
    return createUnique<Window>(width, height, title);
}

void Window::setEventCallback(const std::function<void(Event&)>& callback)
{
    m_data.eventCallback = callback;
}

math::ivec2 Window::getSize() const
{
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    return math::ivec2(width, height);
}

int Window::getWidth() const
{
    return this->getSize().x;
}

int Window::getHeight() const
{
    return this->getSize().y;
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

void Window::setSize(const math::uvec2& size)
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
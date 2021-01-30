#include <core/Window.h>
#include <core/Game.h>
#include <core/Logger.h>
#include <renderer/RenderingContext.h>

#include <events/WindowEvent.h>
#include <events/KeyboardEvent.h>
#include <events/MouseEvent.h>
#include <events/GamepadEvent.h>

#include <GLFW/glfw3.h>

#include <iostream>

namespace Engine
{

static void windowCloseCallback(GLFWwindow* window)
{
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    WindowCloseEvent event;
    data.eventCallback(event);
}

static void framebufferSizeCallback(GLFWwindow* window, int32_t width, int32_t height)
{
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    WindowResizeEvent event(width, height);
    Game::getInstance()->onEvent(event);
}

static void windowMaximizeCallback(GLFWwindow* window, int maximized)
{
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    if (maximized)
    {
        WindowMaximizeEvent event;
        data.eventCallback(event);
    }
    else
    {
        WindowUnmaximizeEvent event;
        data.eventCallback(event);
    }
    
}

static void windowPosCallback(GLFWwindow* window, int xpos, int ypos)
{
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    WindowPositionEvent event(xpos, ypos);
    data.eventCallback(event);
}

static void windowRefreshCallback(GLFWwindow* window)
{
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    WindowRefreshEvent event;
    data.eventCallback(event);
}

static void windowFocusCallback(GLFWwindow* window, int focused)
{
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    if (focused)
    {
        WindowFocusEvent event;
        data.eventCallback(event);
    }
    else
    {
        WindowUnfocusEvent event;
        data.eventCallback(event);
    }
}

static void windowIconifyCallback(GLFWwindow* window, int iconified)
{
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    if (iconified)
    {
        WindowMinimizeEvent event;
        data.eventCallback(event);
    }
    else
    {
        WindowUnminimizeEvent event;
        data.eventCallback(event);
    }
    
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    if (action == GLFW_PRESS)
    {
        KeyPressedEvent event(key, 0, mods);
        data.eventCallback(event);
    }
    else if (action == GLFW_REPEAT)
    {
        KeyPressedEvent event(key, true, mods);
        data.eventCallback(event);
    }
    else if (action == GLFW_RELEASE)
    {
        KeyReleasedEvent event(key);
        data.eventCallback(event);
    }
}

static void characterCallback(GLFWwindow* window, unsigned int codepoint)
{

}

static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    MouseMovedEvent event(xpos, ypos, false);
    data.eventCallback(event);
}

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    if (action == GLFW_PRESS)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        MousePressedEvent event(button, x, y);
        data.eventCallback(event);
    }
    else if (action == GLFW_RELEASE)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        MouseReleasedEvent event(button, x, y);
        data.eventCallback(event);
    }
}

static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    MouseScrollEvent event(xoffset, yoffset);
    data.eventCallback(event);
}

static void mouseEnterCallback(GLFWwindow* window, int entered)
{
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    if (entered)
    {
        MouseEnterEvent event;
        data.eventCallback(event);
    }
    else
    {
        MouseLeaveEvent event;
        data.eventCallback(event);
    }
}

static void joystickCallback(int jid, int event)
{
    WindowData& data = *(WindowData*)glfwGetJoystickUserPointer(jid);
    if (event == GLFW_CONNECTED)
    {
        GamepadConnectEvent event(static_cast<Gamepad::ID>(jid));
        data.eventCallback(event);
    }
    else if (event == GLFW_DISCONNECTED)
    {
        GamepadDisconnectEvent event(static_cast<Gamepad::ID>(jid));
        data.eventCallback(event);
    }
    
}

Window::Window(uint32_t width, uint32_t height, const std::string& title)
    : m_width(width), m_height(height)
{
    bool success = glfwInit();
    if (!success)
    {
        Logger::getCoreLogger()->error("[GLFW] Could not initialize GLFW.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (m_window == nullptr)
    {
        Logger::getCoreLogger()->error("[GLFW] Failed to create window.");
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

    glfwSetJoystickUserPointer(GLFW_JOYSTICK_1, &m_data); // TODO: support for multiple gamepads

    glfwSetJoystickCallback(joystickCallback);
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
}

Owned<Window> Window::create(unsigned int width, unsigned int height, const std::string& title)
{
    return createOwned<Window>(width, height, title);
}

void Window::setEventCallback(const std::function<void(Event&)>& callback)
{
    m_data.eventCallback = callback;
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
    auto image = Image::create(iconPath);
    setIcon(image);
}

void Window::setIcon(const Reference<Image>& image)
{
    GLFWimage icon;
    icon.pixels = reinterpret_cast<unsigned char*>(image->getData());
    icon.width = image->getWidth();
    icon.height = image->getHeight();
    glfwSetWindowIcon(m_window, 1, &icon);
}

void Window::setTitle(const std::string& title)
{
    glfwSetWindowTitle(m_window, title.c_str());
}

void Window::setSize(const math::uvec2& size)
{
    m_width = size.x;
    m_height = size.y;
    
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

}
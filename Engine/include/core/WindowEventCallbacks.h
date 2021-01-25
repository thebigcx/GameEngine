#pragma once

#include <events/EventDispatcher.h>

#include <core/Game.h>
#include <events/Event.h>

#include <GLFW/glfw3.h>

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
    Game::getInstance().onEvent(event);
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
        GamepadConnectEvent event;
        data.eventCallback(event);
    }
    else if (event == GLFW_DISCONNECTED)
    {
        GamepadDisconnectEvent event;
        data.eventCallback(event);
    }
    
}

}
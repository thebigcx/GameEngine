#include <events/EventDispatcher.h>

#include <core/Application.h>
#include <events/Event.h>

#include <GLFW/glfw3.h>

static void window_close_callback(GLFWwindow* window)
{
    WindowCloseEvent event;
    Application::get().onEvent(event);
}

static void framebuffer_size_callback(GLFWwindow* window, int32_t width, int32_t height)
{
    WindowResizeEvent event(width, height);
    Application::get().onEvent(event);
}

static void window_maximize_callback(GLFWwindow* window, int maximized)
{
    if (maximized)
    {
        WindowMaximizeEvent event;
        Application::get().onEvent(event);
    }
    else
    {
        WindowUnmaximizeEvent event;
        Application::get().onEvent(event);
    }
    
}

static void window_pos_callback(GLFWwindow* window, int xpos, int ypos)
{
    WindowPositionEvent event(xpos, ypos);
    Application::get().onEvent(event);
}

static void window_refresh_callback(GLFWwindow* window)
{
    WindowRefreshEvent event;
    Application::get().onEvent(event);
}

static void window_focus_callback(GLFWwindow* window, int focused)
{
    if (focused)
    {
        WindowFocuseEvent event;
        Application::get().onEvent(event);
    }
    else
    {
        WindowUnfocuseEvent event;
        Application::get().onEvent(event);
    }
}

static void window_iconify_callback(GLFWwindow* window, int iconified)
{
    if (iconified)
    {
        WindowMinimizeEvent event;
        Application::get().onEvent(event);
    }
    else
    {
        WindowUnminimizeEvent event;
        Application::get().onEvent(event);
    }
    
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        KeyPressedEvent event(key, 0, mods);
        Application::get().onEvent(event);
    }
    else if (action == GLFW_RELEASE)
    {
        KeyReleasedEvent event(key);
        Application::get().onEvent(event);
    }
}

static void character_callback(GLFWwindow* window, unsigned int codepoint)
{

}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    MouseMovedEvent event(xpos, ypos, false);
    Application::get().onEvent(event);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        MousePressedEvent event(button, x, y);
        Application::get().onEvent(event);
    }
    else if (action == GLFW_RELEASE)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        MouseReleasedEvent event(button, x, y);
        Application::get().onEvent(event);
    }
}

static void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    MouseScrollEvent event(xoffset, yoffset);
    Application::get().onEvent(event);
}

void EventManager::setupCallbacks()
{
    auto window = Application::get().getWindow().getNative();

    glfwSetWindowCloseCallback(window, window_close_callback);
    glfwSetWindowMaximizeCallback(window, window_maximize_callback);
    glfwSetWindowPosCallback(window, window_pos_callback);
    glfwSetWindowRefreshCallback(window, window_refresh_callback);
    glfwSetWindowFocusCallback(window, window_focus_callback);
    glfwSetWindowIconifyCallback(window, window_iconify_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCharCallback(window, character_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);
}
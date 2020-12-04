#include <events/EventDispatcher.h>

#include <core/Application.h>
#include <events/Event.h>

#include <GLFW/glfw3.h>

static void framebuffer_size_callback(GLFWwindow* window, int32_t width, int32_t height)
{
    WindowResizedEvent event(width, height);
    Application::get().onEvent(event);
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

void EventManager::setupCallbacks()
{
    auto& window = Application::get().getWindow();

    glfwSetFramebufferSizeCallback(window.getNative(), framebuffer_size_callback);
    glfwSetKeyCallback(window.getNative(), key_callback);
    glfwSetCharCallback(window.getNative(), character_callback);
    glfwSetCursorPosCallback(window.getNative(), cursor_position_callback);
    glfwSetMouseButtonCallback(window.getNative(), mouse_button_callback);
}
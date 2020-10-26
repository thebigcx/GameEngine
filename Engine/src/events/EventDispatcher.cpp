#include <events/EventDispatcher.h>

#include <core/Application.h>
#include <events/Event.h>

#include <GLFW/glfw3.h>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    Application::get().onWindowResize(width, height);

    EventData data;
    data.window.width = width;
    data.window.height = height;
    Event event(EventType::WindowResize, data);
    Application::get().getEventStack().push(event);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    EventData data;
    data.key.keycode = key;
    data.key.scancode = scancode;
    data.key.action = action;
    data.key.mods = mods;

    EventType type = action == GLFW_PRESS ? EventType::KeyPressed : EventType::KeyReleased;
    Event event(type, data);
    Application::get().getEventStack().push(event);
}

static void character_callback(GLFWwindow* window, unsigned int codepoint)
{
    EventData data;
    data.typekey.code = codepoint;
    Event event(EventType::KeyTyped, data);
    Application::get().getEventStack().push(event);
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    EventData data;
    data.mousepos.x = xpos;
    data.mousepos.y = ypos;
    Event event(EventType::MouseMoved, data);
    Application::get().getEventStack().push(event);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    EventData data;
    data.mouseclick.button = button;
    data.mouseclick.action = (bool)action;

    EventType type = action == GLFW_PRESS ? EventType::MouseButtonPressed : EventType::MouseButtonReleased;
    Event event(type, data);
    Application::get().getEventStack().push(event);
}

void EventDispatcher::setupCallbacks()
{
    auto& window = Application::get().getWindow();

    glfwSetFramebufferSizeCallback(window.getNative(), framebuffer_size_callback);
    glfwSetKeyCallback(window.getNative(), key_callback);
    glfwSetCharCallback(window.getNative(), character_callback);
    glfwSetCursorPosCallback(window.getNative(), cursor_position_callback);
    glfwSetMouseButtonCallback(window.getNative(), mouse_button_callback);
}
#include "core/Mouse.h"

#include <GLFW/glfw3.h>

Vector2i Mouse::m_position;

Vector2i Mouse::getPosition()
{
    auto& window = Application::get().getWindow();
    double x, y;
    glfwGetCursorPos(window.getNative(), &x, &y);
    return Vector2i(x, window.getSize().y - y);
    return m_position;
}
    
bool Mouse::isButtonPressed(Button button)
{
    auto window = Application::get().getWindow().getNative();

    return glfwGetMouseButton(window, button) == GLFW_PRESS;
}

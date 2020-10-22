#include <core/Input.h>

#include <GLFW/glfw3.h>

Vector2i Input::getMousePosition()
{
    auto& window = Application::get().getWindow();
    double x, y;
    glfwGetCursorPos(window.getNative(), &x, &y);
    return Vector2i(x, window.getSize().y - y);
}
    
bool Input::isMousePressed(MouseButton button)
{
    auto window = Application::get().getWindow().getNative();

    return glfwGetMouseButton(window, button) == GLFW_PRESS;
}

bool Input::isKeyPressed(Key key)
{
    return glfwGetKey(Application::get().getWindow().getNative(), key) == GLFW_PRESS;
}
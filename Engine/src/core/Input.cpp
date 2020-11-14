#include <core/Input.h>

#include <GLFW/glfw3.h>

math::ivec2 Input::getMousePosition()
{
    auto& window = Application::get().getWindow();
    double x, y;
    glfwGetCursorPos(window.getNative(), &x, &y);
    return math::ivec2(x, window.getSize().y - y);
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
#include <core/Input.h>
#include <core/Application.h>

math::ivec2 Input::getMousePosition()
{
    auto& window = Application::get().getWindow();
    double x, y;
    glfwGetCursorPos(window.getNative(), &x, &y);
    return math::ivec2(x, window.getSize().y - y);
}
    
bool Input::isMousePressed(MouseButton button)
{
    auto& window = Application::get().getWindow();
    return glfwGetMouseButton(window.getNative(), static_cast<uint32_t>(button));
}

bool Input::isKeyPressed(Key key)
{
    return glfwGetKey(Application::get().getWindow().getNative(), static_cast<uint32_t>(key));
}

bool Input::isGamepadButtonPressed(Gamepad gamepad, GamepadButton button)
{
    int count;
    const unsigned char* btns = glfwGetJoystickButtons(static_cast<uint32_t>(gamepad), &count);
    return btns[static_cast<uint32_t>(button)] == GLFW_PRESS;
}

float Input::getGamepadAxis(Gamepad gamepad, GamepadAxis axis)
{
    int count;
    const float* axes = glfwGetJoystickAxes(static_cast<uint32_t>(gamepad), &count);
    return axes[static_cast<uint32_t>(axis)];
}
#include <core/Gamepad.h>
#include <util/io/FileSystem.h>

#include <GLFW/glfw3.h>

namespace Engine
{

bool Gamepad::isButtonPressed(Gamepad::ID gamepad, Gamepad::Button button)
{
    int count;
    const unsigned char* btns = glfwGetJoystickButtons(static_cast<uint32_t>(gamepad), &count);
    return btns[static_cast<uint32_t>(button)] == GLFW_PRESS;
}

float Gamepad::getAxis(Gamepad::ID gamepad, Gamepad::Axis axis)
{
    int count;
    const float* axes = glfwGetJoystickAxes(static_cast<uint32_t>(gamepad), &count);
    return axes[static_cast<uint32_t>(axis)];
}

void Gamepad::setMappingsFromFile(const std::string& path)
{
    const char* mappings = FileSystem::readFile(path).c_str();
    glfwUpdateGamepadMappings(mappings);
}

}
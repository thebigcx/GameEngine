#include <core/Mouse.h>
#include <core/Game.h>
#include <core/Window.h>

#include <GLFW/glfw3.h>

namespace Engine
{

math::uvec2 Mouse::getPosition()
{
    auto& window = Game::getInstance()->getWindow();
    double x, y;
    glfwGetCursorPos(window.getNative(), &x, &y);
    return math::uvec2(x, window.getSize().y - y);
}
    
bool Mouse::isPressed(Mouse::Button button)
{
    auto& window = Game::getInstance()->getWindow();
    return glfwGetMouseButton(window.getNative(), static_cast<uint32_t>(button));
}

}
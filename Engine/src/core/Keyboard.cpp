#include <core/Keyboard.h>
#include <core/Game.h>
#include <core/Window.h>

#include <GLFW/glfw3.h>

namespace Engine
{

bool Keyboard::isPressed(Keyboard::Key key)
{
    return glfwGetKey(Game::getInstance()->getWindow().getNative(), static_cast<uint32_t>(key));
}

}
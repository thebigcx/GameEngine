#include <core/Cursor.h>
#include <core/Game.h>
#include <core/Window.h>

#include <GLFW/glfw3.h>

namespace Engine
{

Cursor::Cursor()
{

}

Cursor::~Cursor()
{
    glfwDestroyCursor(m_cursor);
}

void Cursor::setCurrent(bool current)
{
    auto& window = Game::getInstance()->getWindow();
    if (current)
    {
        glfwSetCursor(window.getNative(), m_cursor);
    }
    else
    {
        glfwSetCursor(window.getNative(), nullptr);
    }
}

void Cursor::createNativeCursor(NativeCursor cursor)
{
    int shape;
    switch (cursor)
    {
        case Cursor::Arrow: shape = GLFW_ARROW_CURSOR; break;
        case Cursor::IBeam: shape = GLFW_IBEAM_CURSOR; break;
        case Cursor::Crosshair: shape = GLFW_CROSSHAIR_CURSOR; break;
        case Cursor::Hand: shape = GLFW_HAND_CURSOR; break;
        case Cursor::HorizontalResize: shape = GLFW_HRESIZE_CURSOR; break;
        case Cursor::VerticalResize: shape = GLFW_VRESIZE_CURSOR; break;

        default: shape = 0; break;
    };

    m_cursor = glfwCreateStandardCursor(shape);
}

}
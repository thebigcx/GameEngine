#include "FGL/renderer/Renderer.h"

#include <GL/glew.h>

const Window* Renderer::m_currentContext = nullptr;

void Renderer::clear(const Color& color)
{
    glClearColor(color.r / 255, color.g / 255, color.b / 255, color.a / 255);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::display()
{
    if (m_currentContext != nullptr)
    {
        m_currentContext->display();
    }
}

void Renderer::setContext(const Window& window)
{
    window.makeCurrentContext();
    m_currentContext = &window;
}
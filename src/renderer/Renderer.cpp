#include "FGL/renderer/Renderer.h"
#include "FGL/core/Application.h"

#include <GL/glew.h>

const Window* Renderer::m_currentContext = nullptr;
RenderData Renderer::m_data;

void Renderer::init()
{
    glDisable(GL_DEPTH_TEST);

    m_data.shader.create("shaders/default.vert", "shaders/default.frag");
}

void Renderer::clear(const Color& color)
{
    glClearColor(color.r / 255, color.g / 255, color.b / 255, color.a / 255);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::display()
{
    auto& window = Application::get().getWindow();
    window.display();
}

void Renderer::renderBatch(const Batch& batch)
{
    batch.getVertexArray().bind();

    glDrawElements(GL_TRIANGLES, batch.getIndexBuffer().getCount(), GL_UNSIGNED_INT, 0);
}

void Renderer::setContext(const Window& window)
{
    window.makeCurrentContext();
    m_currentContext = &window;
}
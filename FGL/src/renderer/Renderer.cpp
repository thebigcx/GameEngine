#include "renderer/Renderer.h"
#include "core/Application.h"

#include <GL/glew.h>

RenderData Renderer::m_data;

void Renderer::init()
{
    glDisable(GL_DEPTH_TEST);
}

void Renderer::startRender()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::finishRender()
{
    auto& window = Application::get().getWindow();
    glfwSwapBuffers(window.getNative());
}

void Renderer::setClearColor(const Color& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::renderIndexed(const VertexArray& array, RenderStates states)
{
    states.bind();
    states.shader->setUniform("transform", states.transform);
    array.bind();

    glDrawElements(GL_TRIANGLES, array.getIndexBuffer()->getCount(), array.getIndexBuffer()->getIndexType(), 0);
    m_data.drawCalls++;
}
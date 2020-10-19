#include "renderer/Renderer.h"
#include "core/Application.h"

#include <GL/glew.h>

RenderData Renderer::m_data;

void Renderer::init()
{
    glDisable(GL_DEPTH_TEST);

    m_data.shader.create("shaders/default.vert", "shaders/default.frag");
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

void Renderer::renderBatch(const Batch& batch)
{
    batch.getStates().bind();

    batch.getVertexArray().bind();

    int count = batch.getIndexBuffer().getCount();
    GLenum type = batch.getIndexBuffer().getIndexType();

    glDrawElements(GL_TRIANGLES, count, type, 0);
}

void Renderer::renderIndexed(const VertexArray& array, Shader& shader, const glm::mat4& transform)
{
    shader.bind();
    shader.setUniform("transform", transform);
    array.bind();

    glDrawElements(GL_TRIANGLES, array.getIndexBuffer()->getCount(), array.getIndexBuffer()->getIndexType(), 0);
}
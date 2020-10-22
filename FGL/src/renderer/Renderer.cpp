#include "renderer/Renderer.h"
#include "core/Application.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

RenderData Renderer::m_data;

void Renderer::init()
{
    glDisable(GL_DEPTH_TEST);

    m_data.textureShader.create("shaders/texture.vert", "shaders/texture.frag");
    m_data.textureShader.bind();

    auto size = Application::get().getWindow().getSize();

    m_data.projectionMatrix = glm::ortho(0.f, (float)size.x, 0.f, (float)size.y, -1.f, 1.f);
    m_data.textureShader.setUniform("projection", m_data.projectionMatrix);
}

void Renderer::startFrame()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::endFrame()
{
    auto& window = Application::get().getWindow();
    glfwSwapBuffers(window.getNative());
}

void Renderer::setClearColor(const Color& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::render(const VertexArray& array, RenderStates states)
{
    states.bind();
    states.shader->setUniform("transform", states.transform);
    array.bind();

    glDrawElements(GL_TRIANGLES, array.getIndexBuffer()->getCount(), array.getIndexBuffer()->getIndexType(), 0);
    m_data.drawCalls++;
}

void Renderer::render(const VertexArray& array, const glm::mat4& transform, const Texture2D& texture)
{
    m_data.textureShader.bind();
    m_data.textureShader.setUniform("transform", transform);
    BlendMode::Alpha.bind();
    texture.bind();
    array.bind();

    glDrawElements(GL_TRIANGLES, array.getIndexBuffer()->getCount(), array.getIndexBuffer()->getIndexType(), 0);
    m_data.drawCalls++;
}
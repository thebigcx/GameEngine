#include <renderer/Renderer.h>
#include <renderer/MeshFactory.h>
#include <renderer/shader/ShaderFactory.h>

RendererData Renderer::m_data;

void Renderer::init()
{
    RenderCommand::init();
    Renderer2D::init();
    Renderer3D::init();

    m_data.fboMesh = MeshFactory::quadMesh(-1, -1, 1, 1);
    m_data.fboShader = ShaderFactory::framebufferShader();
}

void Renderer::setTarget(const Shared<Framebuffer>& target)
{
    m_data.target = target;
}

void Renderer::startFrame()
{
    m_data.target->bind();
    RenderCommand::clear((uint32_t)RendererBufferType::Color | (uint32_t)RendererBufferType::Depth);
}

void Renderer::endFrame()
{
    m_data.target->unbind();

    RenderCommand::clear((uint32_t)RendererBufferType::Color | (uint32_t)RendererBufferType::Depth);

    glBindTextureUnit(0, m_data.target->getColorAttachment());

    m_data.fboShader->bind();
    m_data.fboMesh->vertexArray->bind();

    RenderCommand::renderIndexed(m_data.fboMesh->vertexArray);
}
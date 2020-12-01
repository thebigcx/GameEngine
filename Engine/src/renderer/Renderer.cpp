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
    m_data.fboShader = ShaderFactory::createShader("hdr");
    m_data.target = Framebuffer::create(1280, 720);
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

void Renderer::windowResize(uint32_t width, uint32_t height)
{
    m_data.target->resize(width, height);
}
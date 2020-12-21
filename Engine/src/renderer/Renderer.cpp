#include <renderer/Renderer.h>
#include <renderer/MeshFactory.h>
#include <renderer/shader/ShaderFactory.h>
#include <core/Application.h>

RendererData Renderer::m_data;
float Renderer::hdrExposure = 1.f;

void Renderer::init()
{
    RenderCommand::init();
    Renderer2D::init();
    Renderer3D::init();

    m_data.fboMesh = MeshFactory::quadMesh(-1, -1, 1, 1);
    m_data.fboShader = ShaderFactory::createShader("hdr");
    
    math::ivec2 windowSize = Application::get().getWindow().getSize();
    m_data.target = Framebuffer::create(windowSize.x, windowSize.y);
}

void Renderer::shutdown()
{
    Renderer2D::shutdown();
    Renderer3D::shutdown();
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

void Renderer::windowResize(WindowResizeEvent& event)
{
    m_data.target->resize(event.getWidth(), event.getHeight());
}
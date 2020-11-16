#include <renderer/Renderer3D.h>
#include <core/Application.h>
#include <math/matrix/matrix_transform.h>
#include <renderer/RenderCommand.h>

Renderer3DData Renderer3D::data;

void Renderer3D::init()
{
    math::ivec2 windowSize = Application::get().getWindow().getSize();
    data.projectionMatrix = math::perspective((float)math::asRadians(60.f), (float)windowSize.x / (float)windowSize.y, 0.1f, 100.f);
    data.projectionMatrix = math::ortho(0.f, (float)windowSize.x, 0.f, (float)windowSize.y, -10.f, 10.f);
}

void Renderer3D::render(const Mesh& mesh, const math::mat4& transform, const Shared<Texture2D>& texture, const Shared<Shader>& shader)
{
    shader->bind();
    shader->setMatrix4("transform", transform);
    shader->setMatrix4("projection", data.projectionMatrix);
    texture->bind();

    mesh.vertexArray->bind();

    RenderCommand::renderIndexed(mesh.vertexArray);
}
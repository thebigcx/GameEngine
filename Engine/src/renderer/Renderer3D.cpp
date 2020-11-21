#include <renderer/Renderer3D.h>
#include <core/Application.h>
#include <math/matrix/matrix_transform.h>
#include <renderer/RenderCommand.h>

Renderer3DData Renderer3D::data;

void Renderer3D::init()
{
    math::ivec2 windowSize = Application::get().getWindow().getSize();
    data.projectionMatrix = math::perspective((float)math::asRadians(45.f), (float)windowSize.x / (float)windowSize.y, 0.1f, 100.f);

    data.modelShader = Shader::createFromFile("shaders/model.glsl");
}

void Renderer3D::render(const Mesh& mesh, const math::mat4& transform, const Shared<Material>& material)
{
    RenderCommand::setDepthTesting(true);

    material->bind();
    material->getShader()->setMatrix4("transform", transform);
    material->getShader()->setMatrix4("projection", data.projectionMatrix);

    mesh.vertexArray->bind();

    RenderCommand::renderIndexed(mesh.vertexArray);
}

void Renderer3D::submit(const Shared<Model>& model, const math::mat4& transform)
{
    for (auto& mesh : model->meshes)
    {
        render(*mesh, transform, mesh->material);
    }
}
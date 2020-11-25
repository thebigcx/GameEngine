#include <renderer/Renderer3D.h>
#include <core/Application.h>
#include <maths/maths/matrix/matrix_transform.h>
#include <renderer/RenderCommand.h>
#include <renderer/shader/ShaderFactory.h>

Renderer3DData Renderer3D::data;

void Renderer3D::init()
{
    math::ivec2 windowSize = Application::get().getWindow().getSize();
    data.projectionMatrix = math::perspective((float)math::asRadians(45.f), (float)windowSize.x / (float)windowSize.y, 0.1f, 100.f);

    data.modelShader = ShaderFactory::lightingShader();
}

void Renderer3D::render(const Mesh& mesh, const math::mat4& transform)
{
    RenderCommand::setDepthTesting(true);

    mesh.material->bind();
    mesh.material->getShader()->setMatrix4("transform", transform);
    mesh.material->getShader()->setMatrix4("projection", data.projectionMatrix);
    mesh.material->getShader()->setFloat("material.shininess", mesh.material->shininess);

    mesh.vertexArray->bind();

    RenderCommand::renderIndexed(mesh.vertexArray);
}

void Renderer3D::submit(const Shared<Model>& model, const math::mat4& transform)
{
    if (!data.sceneStarted)
    {
        std::cout << "beginScene() must be called before executing draw calls!\n";
    }

    RenderCommand::setDepthTesting(true);

    for (auto& mesh : model->meshes)
    {
        mesh->material->bind();
        mesh->material->getShader()->setMatrix4("transform", transform);
        mesh->material->getShader()->setMatrix4("view", data.camera->getViewMatrix());
        mesh->material->getShader()->setMatrix4("projection", data.projectionMatrix);
        mesh->material->getShader()->setFloat("material.shininess", mesh->material->shininess);

        mesh->vertexArray->bind();

        RenderCommand::renderIndexed(mesh->vertexArray);
    }
}

void Renderer3D::setLights(const LightSetup& setup)
{
    data.modelShader->bind();

    data.modelShader->setFloat3("dirLight.direction", setup.getDirectionalLight().direction);
    data.modelShader->setFloat3("dirLight.ambient", setup.getDirectionalLight().ambient);
    data.modelShader->setFloat3("dirLight.diffuse", setup.getDirectionalLight().diffuse);
    data.modelShader->setFloat3("dirLight.specular", setup.getDirectionalLight().specular);

    auto pointLights = setup.getPointLights();
    for (int i = 0; i < pointLights.size(); i++)
    {
        std::string index = std::to_string(i);
        data.modelShader->setFloat3("pointLights[" + index + "].position",  pointLights[i].position);
        data.modelShader->setFloat3("pointLights[" + index + "].ambient",   pointLights[i].ambient);
        data.modelShader->setFloat3("pointLights[" + index + "].diffuse",   pointLights[i].diffuse);
        data.modelShader->setFloat3("pointLights[" + index + "].specular",  pointLights[i].specular);
        data.modelShader->setFloat("pointLights[" + index + "].constant",  pointLights[i].constant);
        data.modelShader->setFloat("pointLights[" + index + "].linear",    pointLights[i].linear);
        data.modelShader->setFloat("pointLights[" + index + "].quadratic", pointLights[i].quadratic);
    }

    Renderer3D::data.modelShader->setInt("numPointLights", pointLights.size());
}

void Renderer3D::beginScene(PerspectiveCamera& camera)
{
    if (data.sceneStarted)
    {
        std::cout << "Renderer3D::beginScene() called before endScene()!\n";
    }

    data.sceneStarted = true;
    data.camera = &camera;

    data.modelShader->bind();
    data.modelShader->setFloat3("viewPos", data.camera->getPosition());
}

void Renderer3D::endScene()
{
    data.sceneStarted = false;
}
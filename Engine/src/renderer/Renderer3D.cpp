#include <renderer/Renderer3D.h>
#include <core/Application.h>
#include <maths/matrix/matrix_transform.h>
#include <renderer/RenderCommand.h>
#include <renderer/shader/ShaderFactory.h>
#include <renderer/MeshFactory.h>

Renderer3DData Renderer3D::data;

void Renderer3D::init()
{
    math::ivec2 windowSize = Application::get().getWindow().getSize();
    data.projectionMatrix = math::perspective((float)math::radians(45.f), (float)windowSize.x / (float)windowSize.y, 0.1f, 100.f);

    data.modelShader = ShaderFactory::lightingShader();

    data.hdrBuffer = Framebuffer::create(1280, 720);
    data.quadMesh = MeshFactory::quadMesh(-1, -1, 1, 1);
    data.hdrShader = Shader::createFromFile("Engine/src/renderer/shader/default/hdr.glsl");
}

void Renderer3D::beginScene(PerspectiveCamera& camera)
{
    if (data.sceneStarted)
    {
        std::cout << "beginScene() called before endScene()!\n";
    }

    data.sceneStarted = true;
    data.camera = &camera;

    data.modelShader->bind();
    data.modelShader->setFloat3("viewPos", data.camera->getPosition());

    data.hdrBuffer->bind();
    RenderCommand::clear((uint32_t)RendererBufferType::Color | (uint32_t)RendererBufferType::Depth);
}

void Renderer3D::endScene()
{
    data.sceneStarted = false;

    data.hdrBuffer->unbind();

    RenderCommand::clear((uint32_t)RendererBufferType::Color | (uint32_t)RendererBufferType::Depth);

    glBindTextureUnit(0, data.hdrBuffer->getColorAttachment());

    data.hdrShader->bind();
    data.quadMesh->vertexArray->bind();

    RenderCommand::renderIndexed(data.quadMesh->vertexArray);
}

void Renderer3D::submit(const Shared<Mesh>& mesh, const math::mat4& transform)
{
    if (!data.sceneStarted)
    {
        std::cout << "beginScene() must be called before executing draw calls!\n";
    }

    RenderCommand::setDepthTesting(true);

    mesh->material->bind();
    mesh->material->getShader()->setMatrix4("view", data.camera->getViewMatrix());
    mesh->material->getShader()->setMatrix4("transform", transform);
    mesh->material->getShader()->setMatrix4("projection", data.projectionMatrix);
    mesh->material->getShader()->setFloat("material.shininess", mesh->material->shininess);

    mesh->vertexArray->bind();

    RenderCommand::renderIndexed(mesh->vertexArray);
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

    auto& dirLight = setup.getDirectionalLight();

    data.modelShader->setFloat("skyLight", setup.getSkyLight());

    data.modelShader->setFloat3("dirLight.direction", dirLight.direction);
    data.modelShader->setFloat3("dirLight.color", dirLight.color);
    data.modelShader->setFloat("dirLight.intensity", dirLight.intensity);
    data.modelShader->setFloat("dirLight.specular", dirLight.specular);

    auto& pointLights = setup.getPointLights();
    for (int i = 0; i < pointLights.size(); i++)
    {
        std::string index = std::to_string(i);

        auto& light = pointLights[i];

        data.modelShader->setFloat3("pointLights[" + index + "].position",  light.position);
        data.modelShader->setFloat3("pointLights[" + index + "].color",   light.color);
        data.modelShader->setFloat("pointLights[" + index + "].intensity",  light.intensity);
        data.modelShader->setFloat("pointLights[" + index + "].specular",  light.specular);
        data.modelShader->setFloat("pointLights[" + index + "].attenuation", light.attenuation);
    }

    auto& spotLights = setup.getSpotLights();
    for (int i = 0; i < spotLights.size(); i++)
    {
        std::string index = std::to_string(i);

        auto& light = spotLights[i];

        data.modelShader->setFloat3("spotLights[" + index + "].position",  light.position);
        data.modelShader->setFloat3("spotLights[" + index + "].direction",  light.direction);
        data.modelShader->setFloat3("spotLights[" + index + "].color",   light.color);
        data.modelShader->setFloat("spotLights[" + index + "].intensity",   light.intensity);
        data.modelShader->setFloat("spotLights[" + index + "].specular",  light.specular);
        data.modelShader->setFloat("spotLights[" + index + "].attenuation", light.attenuation);
        data.modelShader->setFloat("spotLights[" + index + "].cutoff", light.cutoff);
        data.modelShader->setFloat("spotLights[" + index + "].outerCutoff", light.outerCutoff);
    }

    Renderer3D::data.modelShader->setInt("numPointLights", pointLights.size());
    Renderer3D::data.modelShader->setInt("numSpotLights", spotLights.size());
}
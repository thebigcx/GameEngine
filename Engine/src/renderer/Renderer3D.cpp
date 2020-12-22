#include <renderer/Renderer3D.h>
#include <core/Application.h>
#include <maths/matrix/matrix_transform.h>
#include <renderer/RenderCommand.h>
#include <renderer/shader/ShaderFactory.h>
#include <renderer/MeshFactory.h>
#include <renderer/Renderer.h>
#include <renderer/Assets.h>

Renderer3DData Renderer3D::data;

void Renderer3D::init()
{
    math::ivec2 windowSize = Application::get().getWindow().getSize();
    
    //data.modelShader = Shader::createFromFile("Engine/src/renderer/shader/default/pbr.glsl");
    Assets::add<Shader>("pbr", Shader::createFromFile("Engine/src/renderer/shader/default/pbr.glsl"));
    data.modelShader = Assets::get<Shader>("pbr");

    data.matrixData = UniformBuffer::create(sizeof(math::mat4) * 2, 0);

    data.lightingData = UniformBuffer::create(sizeof(DirectionalLight)
                                            + sizeof(PointLight) * 64
                                            + sizeof(SpotLight) * 64
                                            + sizeof(uint32_t)
                                            + sizeof(uint32_t)
                                            + sizeof(math::vec3)
                                            + sizeof(float), 1);

    std::array<std::string, 6> skyboxFaces = {
        "Sandbox/assets/skybox/right.jpg",
        "Sandbox/assets/skybox/left.jpg",
        "Sandbox/assets/skybox/top.jpg",
        "Sandbox/assets/skybox/bottom.jpg",
        "Sandbox/assets/skybox/front.jpg",
        "Sandbox/assets/skybox/back.jpg"
    };

    data.environment = Skybox::create(skyboxFaces);
    data.skyboxMesh = MeshFactory::skyboxMesh();
    data.skyboxShader = Shader::createFromFile("Engine/src/renderer/shader/default/skybox.glsl");
}

void Renderer3D::shutdown()
{
    
}

void Renderer3D::beginScene(PerspectiveCamera& camera)
{
    if (data.sceneStarted)
    {
        Logger::getCoreLogger()->error("beginScene() must be called before endScene()!");
    }

    data.sceneStarted = true;

    data.matrixData->setData(math::buffer(camera.getProjectionMatrix()), sizeof(math::mat4), 0);
    data.matrixData->setData(math::buffer(camera.getViewMatrix()), sizeof(math::mat4), sizeof(math::mat4));

    data.modelShader->bind();
    data.modelShader->setFloat3("cameraPos", camera.getPosition());
}

void Renderer3D::beginScene(EditorCamera& camera)
{
    if (data.sceneStarted)
    {
        Logger::getCoreLogger()->error("beginScene() must be called before endScene()!");
    }

    data.sceneStarted = true;

    data.matrixData->setData(math::buffer(camera.getProjectionMatrix()), sizeof(math::mat4), 0);
    data.matrixData->setData(math::buffer(camera.getViewMatrix()), sizeof(math::mat4), sizeof(math::mat4));

    data.modelShader->bind();
    data.modelShader->setFloat3("cameraPos", camera.getPosition());
}

void Renderer3D::endScene()
{
    data.sceneStarted = false;

    /*glDepthFunc(GL_LEQUAL);
    data.skyboxShader->bind();
    data.skyboxMesh->vertexArray->bind();
    data.environment->getCubemap()->bind();
    RenderCommand::renderIndexed(data.skyboxMesh->vertexArray);
    glDepthFunc(GL_LESS);*/
}

void Renderer3D::submit(const Shared<Mesh>& mesh, const math::mat4& transform)
{
    if (!data.sceneStarted)
    {
        Logger::getCoreLogger()->error("beginScene() must be called before executing draw calls!");
    }

    RenderCommand::setDepthTesting(true);

    if (!mesh->materials.at(0)->shader)
    {
        return;
    }

    mesh->materials.at(0)->bind();
    mesh->materials.at(0)->shader->setMatrix4("transform", transform);
    mesh->materials.at(0)->shader->setFloat("exposure", Renderer::hdrExposure);

    mesh->vertexArray->bind();

    RenderCommand::renderIndexed(mesh->vertexArray);
}

void Renderer3D::submit(const Shared<Model>& model, const math::mat4& transform)
{
    if (!data.sceneStarted)
    {
        Logger::getCoreLogger()->error("beginScene() must be called before executing draw calls!");
    }

    RenderCommand::setDepthTesting(true);

    for (auto& mesh : model->meshes)
    {
        mesh->materials.at(0)->bind();
        mesh->materials.at(0)->shader->setMatrix4("transform", transform);
        mesh->materials.at(0)->shader->setFloat("exposure", Renderer::hdrExposure);

        mesh->vertexArray->bind();

        RenderCommand::renderIndexed(mesh->vertexArray);
    }
}

void Renderer3D::submit(const Shared<Mesh>& mesh, const math::mat4& transform, const Shared<Material>& material)
{
    if (!data.sceneStarted)
    {
        Logger::getCoreLogger()->error("beginScene() must be called before executing draw calls!");
    }

    if (!material->shader)
    {
        return;
    }

    RenderCommand::setDepthTesting(true);

    material->bind();
    material->shader->setMatrix4("transform", transform);

    mesh->vertexArray->bind();

    RenderCommand::renderIndexed(mesh->vertexArray);
}

void Renderer3D::setLights(const LightSetup& setup)
{
    data.modelShader->bind();

    data.modelShader->setFloat("skyLight", setup.getSkyLight());
    
    /*DirectionalLight dirLight = setup.getDirectionalLight();
    data.modelShader->setFloat3("dirLight.direction", dirLight.direction);
    data.modelShader->setFloat3("dirLight.color", dirLight.color);
    data.modelShader->setFloat("dirLight.intensity", dirLight.intensity);*/

    auto& pointLights = setup.getPointLights();
    for (unsigned int i = 0; i < pointLights.size(); i++)
    {
        std::string index = std::to_string(i);

        auto& light = pointLights[i];

        data.modelShader->setFloat3("pointLights[" + index + "].position",  light.position);
        data.modelShader->setFloat3("pointLights[" + index + "].radiance",   light.radiance);
        data.modelShader->setFloat("pointLights[" + index + "].intensity",  light.intensity);
    }

    auto& spotLights = setup.getSpotLights();
    for (unsigned int i = 0; i < spotLights.size(); i++)
    {
        std::string index = std::to_string(i);

        auto& light = spotLights[i];

        /*data.modelShader->setFloat3("spotLights[" + index + "].position",  light.position);
        data.modelShader->setFloat3("spotLights[" + index + "].direction",  light.direction);
        data.modelShader->setFloat3("spotLights[" + index + "].color",   light.radiance);
        data.modelShader->setFloat("spotLights[" + index + "].intensity",   light.intensity);
        data.modelShader->setFloat("spotLights[" + index + "].attenuation", light.attenuation);
        data.modelShader->setFloat("spotLights[" + index + "].cutoff", light.cutoff);
        data.modelShader->setFloat("spotLights[" + index + "].outerCutoff", light.outerCutoff);*/
    }

    Assets::get<Shader>("pbr")->setInt("numPointLights", pointLights.size());
    //Assets::get<Shader>("pbr")->setInt("numSpotLights", spotLights.size());
}

void Renderer3D::setEnvironment(const Shared<Skybox>& environment)
{
    data.environment = environment;
}
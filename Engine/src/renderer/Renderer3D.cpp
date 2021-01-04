#include <renderer/Renderer3D.h>
#include <core/Application.h>
#include <maths/matrix/matrix_transform.h>
#include <renderer/RenderCommand.h>
#include <renderer/shader/ShaderFactory.h>
#include <renderer/MeshFactory.h>
#include <renderer/Renderer.h>
#include <renderer/Assets.h>

namespace Engine
{

Renderer3DData Renderer3D::data;

void Renderer3D::init()
{
    math::ivec2 windowSize = Application::get().getWindow().getSize();
    
    Assets::add<Shader>("pbr", Shader::createFromFile("Engine/src/renderer/shader/default/pbr.glsl"));

    data.matrixData = UniformBuffer::create(sizeof(math::mat4) * 2, 0);

    data.lightingData = UniformBuffer::create(sizeof(DirectionalLight)
                                            + sizeof(PointLight) * 64
                                            + sizeof(SpotLight) * 64
                                            + sizeof(uint32_t)
                                            + sizeof(uint32_t)
                                            + sizeof(math::vec3)
                                            + sizeof(float), 1);

    /*std::array<std::string, 6> skyboxFaces = {
        "Sandbox/assets/skybox/right.jpg",
        "Sandbox/assets/skybox/left.jpg",
        "Sandbox/assets/skybox/top.jpg",
        "Sandbox/assets/skybox/bottom.jpg",
        "Sandbox/assets/skybox/front.jpg",
        "Sandbox/assets/skybox/back.jpg"
    };

    data.environment = Skybox::create(skyboxFaces);*/
    data.skyboxMesh = MeshFactory::skyboxMesh();

    data.skyboxShader = Shader::createFromFile("Engine/src/renderer/shader/default/skybox.glsl");
    Assets::add<Shader>("skybox", data.skyboxShader);

    data.shadowMap = Texture2D::create(1024, 1024, GL_DEPTH_COMPONENT16, true, false);
    data.shadowMapFramebuffer = Framebuffer::create(data.shadowMap, GL_DEPTH_ATTACHMENT);
    data.shadowMapFramebuffer->drawBuffer((uint32_t)ColorBuffer::None);
    data.shadowMapFramebuffer->readBuffer((uint32_t)ColorBuffer::None);
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

    data.cameraPos = camera.getPosition();
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

    data.cameraPos = camera.getPosition();
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

    if (!mesh->material->shader)
    {
        return;
    }

    mesh->material->bind();
    mesh->material->shader->setMatrix4("transform", transform);
    mesh->material->shader->setFloat("exposure", Renderer::hdrExposure);
    mesh->material->shader->setFloat3("cameraPos", data.cameraPos);

    setLightingUniforms(mesh->material->shader);

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

    Shared<Material> lastMaterial = nullptr;
    for (auto& mesh : model->meshes)
    {
        if (mesh->material != lastMaterial)
        {
            mesh->material->bind();
            mesh->material->shader->setMatrix4("transform", transform);
            mesh->material->shader->setFloat("exposure", Renderer::hdrExposure);
            mesh->material->shader->setFloat3("cameraPos", data.cameraPos);
            
            setLightingUniforms(mesh->material->shader);

            lastMaterial = mesh->material;
        }

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
    material->shader->setFloat("exposure", Renderer::hdrExposure);
    material->shader->setFloat3("cameraPos", data.cameraPos);

    setLightingUniforms(material->shader);

    mesh->vertexArray->bind();

    RenderCommand::renderIndexed(mesh->vertexArray);
}

void Renderer3D::setLightingUniforms(const Shared<Shader>& shader)
{
    uint32_t pointLights = 0;
    for (auto& light : data.lights)
    {
        if (dynamic_cast<const PointLight*>(light))
        {
            light->setShaderUniforms(shader, pointLights);
            pointLights++;
        }
        else if (dynamic_cast<const SkyLight*>(light) || dynamic_cast<const DirectionalLight*>(light))
        {
            light->setShaderUniforms(shader, 0);
        }
        
    }

    shader->setInt("numPointLights", pointLights);
}

void Renderer3D::removeLight(const BaseLight* light)
{
    for (std::vector<const BaseLight*>::iterator it = data.lights.begin(); it != data.lights.end(); it++)
    {
        if (*(it.base()) == light)
        {
            data.lights.erase(it);
        }
    }
}

void Renderer3D::setEnvironment(const Shared<Skybox>& environment)
{
    data.environment = environment;
}

}
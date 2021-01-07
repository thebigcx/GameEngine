#include <renderer/Renderer3D.h>
#include <core/Application.h>
#include <maths/matrix/matrix_transform.h>
#include <renderer/RenderCommand.h>
#include <renderer/shader/ShaderFactory.h>
#include <renderer/MeshFactory.h>
#include <renderer/Renderer.h>
#include <renderer/Assets.h>
#include <util/Timer.h>

namespace Engine
{

void Renderer3D::init()
{
    math::ivec2 windowSize = Application::get().getWindow().getSize();
    
    Assets::add<Shader>("pbr", Shader::createFromFile("Engine/src/renderer/shader/default/pbr.glsl"));
    Assets::add<Shader>("instancepbr", Shader::createFromFile("Engine/src/renderer/shader/default/instancepbr.glsl"));

    s_data.matrixData = UniformBuffer::create(sizeof(math::mat4) * 2, 0);
    s_data.matrixData->setBlockDeclaration(*Assets::get<Shader>("pbr"));

    s_data.skyboxMesh = MeshFactory::skyboxMesh();
    s_data.environment = EnvironmentMap::create("Sandbox/assets/environment.hdr");
    s_data.environmentShader = Shader::createFromFile("Engine/src/renderer/shader/default/environmentMap.glsl");

    s_data.lightingData = UniformBuffer::create(sizeof(DirectionalLight)
                                            + sizeof(PointLight) * 64
                                            + sizeof(SpotLight) * 64
                                            + sizeof(uint32_t)
                                            + sizeof(uint32_t)
                                            + sizeof(math::vec3)
                                            + sizeof(float), 1);

    s_data.shadowMap = Texture2D::create(1024, 1024, GL_DEPTH_COMPONENT16, true, false);
    s_data.shadowMapFramebuffer = Framebuffer::create(s_data.shadowMap, Attachment::Depth);
    s_data.shadowMapFramebuffer->drawBuffer((uint32_t)ColorBuffer::None);
    s_data.shadowMapFramebuffer->readBuffer((uint32_t)ColorBuffer::None);
}

void Renderer3D::shutdown()
{
    
}

void Renderer3D::startBatch()
{
    s_data.renderObjects.clear();
}

void Renderer3D::flushBatch()
{
    RenderCommand::setDepthTesting(true);

    s_data.environment->getEnvMap()->bind(6);
    s_data.environment->getPrefilter()->bind(7);
    s_data.environment->getBRDF()->bind(8);

    for (auto& group : s_data.renderObjects)
    {
        group.first->bind();
        group.first->shader->setFloat("exposure", Renderer::hdrExposure);
        group.first->shader->setFloat3("cameraPos", s_data.cameraPos);

        setLightingUniforms(group.first->shader);

        for (auto& renderObject : group.second)
        {
            group.first->shader->setMatrix4("transform", renderObject.transform);

            renderObject.mesh->vertexArray->bind();

            RenderCommand::renderIndexed(renderObject.mesh->vertexArray);
        }
    }
}

void Renderer3D::nextBatch()
{
    flushBatch();
    startBatch();
}

void Renderer3D::beginScene(PerspectiveCamera& camera)
{
    if (s_data.sceneStarted)
    {
        Logger::getCoreLogger()->error("beginScene() must be called before endScene()!");
    }

    s_data.sceneStarted = true;

    s_data.matrixData->setVariable("projection", math::buffer(camera.getProjectionMatrix()), sizeof(math::mat4));
    s_data.matrixData->setVariable("view", math::buffer(camera.getViewMatrix()), sizeof(math::mat4));

    s_data.cameraPos = camera.getPosition();

    startBatch();
}

void Renderer3D::beginScene(EditorCamera& camera)
{
    if (s_data.sceneStarted)
    {
        Logger::getCoreLogger()->error("beginScene() must be called before endScene()!");
    }

    s_data.sceneStarted = true;

    s_data.matrixData->setVariable("projection", math::buffer(camera.getProjectionMatrix()), sizeof(math::mat4));
    s_data.matrixData->setVariable("view", math::buffer(camera.getViewMatrix()), sizeof(math::mat4));

    s_data.cameraPos = camera.getPosition();

    startBatch();
}

void Renderer3D::endScene()
{
    s_data.sceneStarted = false;

    flushBatch();

    glDepthFunc(GL_LEQUAL);
    s_data.environmentShader->bind();
    s_data.skyboxMesh->vertexArray->bind();
    s_data.environment->getEnvMap()->bind();
    RenderCommand::renderIndexed(s_data.skyboxMesh->vertexArray);
    glDepthFunc(GL_LESS);
}

void Renderer3D::submit(const Shared<Mesh>& mesh, const math::mat4& transform)
{
    if (!s_data.sceneStarted)
    {
        Logger::getCoreLogger()->error("beginScene() must be called before executing draw calls!");
    }

    if (!mesh->material->shader)
    {
        return;
    }

    if (s_data.renderObjects.find(mesh->material) == s_data.renderObjects.end())
    {
        s_data.renderObjects.emplace(std::make_pair(mesh->material, std::vector<RenderObject>()));
    }

    s_data.renderObjects.at(mesh->material).push_back({ mesh, transform });
}

void Renderer3D::submit(const Shared<Model>& model, const math::mat4& transform)
{
    for (auto& mesh : model->meshes)
    {
        submit(mesh, transform);
    }
}

void Renderer3D::submit(const Shared<Mesh>& mesh, const math::mat4& transform, const Shared<Material>& material)
{
    if (!s_data.sceneStarted)
    {
        Logger::getCoreLogger()->error("beginScene() must be called before executing draw calls!");
    }

    if (!material->shader)
    {
        return;
    }

    if (s_data.renderObjects.find(material) == s_data.renderObjects.end())
    {
        s_data.renderObjects.emplace(std::make_pair(material, std::vector<RenderObject>()));
    }

    s_data.renderObjects.at(material).push_back({ mesh, transform });
}

void Renderer3D::submit(const Shared<InstancedRenderer>& instance)
{
    if (!s_data.sceneStarted)
    {
        Logger::getCoreLogger()->error("beginScene() must be called before executing draw calls!");
    }

    RenderCommand::setDepthTesting(true);

    Shared<Material> lastMaterial = nullptr;
    for (auto& mesh : instance->getInstance())
    {
        if (mesh->material != lastMaterial)
        {
            mesh->material->bind();
            mesh->material->shader->setFloat("exposure", Renderer::hdrExposure);
            mesh->material->shader->setFloat3("cameraPos", s_data.cameraPos);
            
            setLightingUniforms(mesh->material->shader);

            lastMaterial = mesh->material;
        }

        mesh->vertexArray->bind();

        RenderCommand::renderInstanced(mesh->vertexArray, instance->getInstances().size());
    }
}

void Renderer3D::setLightingUniforms(const Shared<Shader>& shader)
{
    uint32_t pointLights = 0;
    uint32_t directionalLights = 0;
    for (auto& light : s_data.lights)
    {
        if (dynamic_cast<const PointLight*>(light))
        {
            light->setShaderUniforms(shader, pointLights);
            pointLights++;
        }
        else if (dynamic_cast<const DirectionalLight*>(light))
        {
            light->setShaderUniforms(shader, 0);
            directionalLights++;
        }
        else if (dynamic_cast<const SkyLight*>(light))
        {
            light->setShaderUniforms(shader, 0);
        }
        
    }

    shader->setInt("numPointLights", pointLights);
    shader->setInt("usingDirectionalLight", directionalLights); // TODO: multiple directional lights
}

void Renderer3D::removeLight(const BaseLight* light)
{
    for (std::vector<const BaseLight*>::iterator it = s_data.lights.begin(); it != s_data.lights.end(); it++)
    {
        if (*(it.base()) == light)
        {
            s_data.lights.erase(it);
        }
    }
}

void Renderer3D::setEnvironment(const Shared<Skybox>& environment)
{
    
}

}
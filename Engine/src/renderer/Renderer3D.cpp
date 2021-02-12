#include <renderer/Renderer3D.h>
#include <core/Game.h>
#include <maths/matrix/matrix_transform.h>
#include <renderer/RenderCommand.h>
#include <renderer/MeshFactory.h>
#include <renderer/Renderer.h>
#include <renderer/Assets.h>
#include <util/Timer.h>
#include <maths/vector/vec_func.h>
#include <util/io/FileSystem.h>

namespace Engine
{

void Renderer3D::init()
{
    math::ivec2 windowSize = Game::getInstance()->getWindow().getSize();

    s_data.environmentShader = Assets::get<Shader>("EngineIBL_Environment");
    s_data.shadowMapShader = Assets::get<Shader>("EngineShadow_Map");

    s_data.matrixData = UniformBuffer::create(sizeof(math::mat4) * 2, 0);
    s_data.matrixData->setBlockDeclaration(*Assets::get<Shader>("EnginePBR_Static"));

    s_data.skyboxMesh = MeshFactory::skyboxMesh();
    s_data.environment = EnvironmentMap::create("Sandbox/assets/environment.hdr");

    Framebuffer::Specification spec = {
        1024, 1024,
        {
            { Framebuffer::Attachment::Depth, Framebuffer::TextureSpecification(SizedTextureFormat::Depth24Stencil8) }
        }
    };

    s_data.shadowMap = Texture2D::create(1024, 1024, SizedTextureFormat::Depth16, false, false);
    s_data.shadowMapFramebuffer = Framebuffer::create(spec);
    s_data.shadowMapFramebuffer->drawBuffer((uint32_t)ColorBuffer::None);
    s_data.shadowMapFramebuffer->readBuffer((uint32_t)ColorBuffer::None);

    s_data.lightProjection = math::ortho(-10.f, 10.f, -10.f, 10.f, 1.f, 7.5f);
    s_data.lightView = math::lookAt(math::vec3(-2.0f, 4.0f, -1.0f), 
                                    math::vec3( 0.0f, 0.0f,  0.0f), 
                                    math::vec3( 0.0f, 1.0f,  0.0f));

    s_data.lightMatrix = s_data.lightProjection * s_data.lightView;
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
    
    // Shadows
    renderShadows();

    s_data.environment->getIrradiance()->bind(7);
    s_data.environment->getPrefilter()->bind(8);
    s_data.environment->getBRDF()->bind(9);
    s_data.shadowMap->bind(10);

    for (auto& group : s_data.renderObjects)
    {
        group.first->bind();
        group.first->shader->setFloat3("uCameraPos", s_data.cameraPos);
        group.first->shader->setMatrix4("uLightSpaceMatrix", s_data.lightMatrix);

        setLightingUniforms(group.first->shader);

        for (auto& renderObject : group.second)
        {
            group.first->shader->setMatrix4("uTransform", renderObject.transform);
            renderObject.mesh->vertexArray->bind();

            RenderCommand::renderIndexed(renderObject.mesh->vertexArray);
        }
    }
}

void Renderer3D::renderShadows()
{
    RenderCommand::setDepthTesting(true);
    
    auto prevFbo = Framebuffer::getCurrentBoundFramebuffer();

    // Shadows
    RenderCommand::setViewport(0, 0, 1024, 1024);
    s_data.shadowMapFramebuffer->bind();
    RenderCommand::clear((uint32_t)RendererBufferType::Depth);

    s_data.shadowMapShader->bind();
    s_data.shadowMapShader->setMatrix4("uLightSpaceMatrix", s_data.lightMatrix);

    for (auto& group : s_data.renderObjects)
    {
        for (auto& renderObject : group.second)
        {
            renderObject.mesh->vertexArray->bind();

            s_data.shadowMapShader->setMatrix4("uTransform", renderObject.transform);

            RenderCommand::renderIndexed(renderObject.mesh->vertexArray);
        }
    }

    s_data.shadowMapFramebuffer->unbind();

    prevFbo->bind();
}

void Renderer3D::nextBatch()
{
    flushBatch();
    startBatch();
}

void Renderer3D::beginScene(EditorCamera& camera)
{
    if (s_data.sceneStarted)
    {
        Logger::getCoreLogger()->error("beginScene() must be called before endScene()!");
    }

    s_data.sceneStarted = true;

    s_data.matrixData->setVariable("uProjection", math::buffer(camera.getProjectionMatrix()), sizeof(math::mat4));
    s_data.matrixData->setVariable("uView", math::buffer(camera.getViewMatrix()), sizeof(math::mat4));

    s_data.cameraPos = camera.getPosition();

    startBatch();
}

void Renderer3D::beginScene(Camera& camera, const math::mat4& transform)
{
    if (s_data.sceneStarted)
    {
        Logger::getCoreLogger()->error("beginScene() must be called before endScene()!");
    }

    s_data.sceneStarted = true;

    s_data.matrixData->setVariable("uProjection", math::buffer(camera.getProjectionMatrix()), sizeof(math::mat4));
    s_data.matrixData->setVariable("uView", math::buffer(math::inverse<float>(transform)), sizeof(math::mat4));

    s_data.cameraPos = transform[3];

    startBatch();
}

void Renderer3D::endScene()
{
    s_data.sceneStarted = false;

    flushBatch();

    if (s_data.usingSkybox)
    {
        glDepthFunc(GL_LEQUAL);
        s_data.environmentShader->bind();
        s_data.skyboxMesh->vertexArray->bind();
        s_data.environment->getEnvMap()->bind();
        RenderCommand::renderIndexed(s_data.skyboxMesh->vertexArray);
        glDepthFunc(GL_LESS);
    }
}

void Renderer3D::submit(const Reference<Mesh>& mesh, const math::mat4& transform)
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

void Renderer3D::submit(const Reference<Model>& model, const math::mat4& transform)
{
    for (auto& mesh : model->meshes)
    {
        submit(mesh, transform);
    }
}

void Renderer3D::submit(const Reference<Mesh>& mesh, const math::mat4& transform, const Reference<Material>& material)
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

void Renderer3D::submit(const Reference<InstancedRenderer>& instance)
{
    if (!s_data.sceneStarted)
    {
        Logger::getCoreLogger()->error("beginScene() must be called before executing draw calls!");
    }

    RenderCommand::setDepthTesting(true);

    Reference<Material> lastMaterial = nullptr;
    for (auto& mesh : instance->getInstance())
    {
        if (mesh->material != lastMaterial)
        {
            mesh->material->bind();
            mesh->material->shader->setFloat("uExposure", Renderer::hdrExposure);
            
            setLightingUniforms(mesh->material->shader);

            lastMaterial = mesh->material;
        }

        mesh->vertexArray->bind();

        RenderCommand::renderInstanced(mesh->vertexArray, instance->getInstances().size());
    }
}

void Renderer3D::submitOutline(const Reference<Mesh>& mesh, const math::mat4& transform, const math::vec3& outlineColor)
{
    /*
    mesh->vertexArray->bind();

    float scale = math::dist(s_data.cameraPos, math::vec3(transform[3])) / 100.f + 1;

    Assets::get<Shader>("outline")->bind();
    Assets::get<Shader>("outline")->setMatrix4("transform", math::scale(transform, math::vec3(scale, scale, scale)));
    Assets::get<Shader>("outline")->setFloat3("outlineColor", outlineColor);

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LEQUAL);
    RenderCommand::renderIndexed(mesh->vertexArray);
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
    */
}

void Renderer3D::setLightingUniforms(const Reference<Shader>& shader)
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

            /*auto dirLight = static_cast<const DirectionalLight*>(light);
            s_data.lightView = math::lookAt(dirLight->direction * 10, dirLight->direction, math::vec3(0, 1, 0));
            s_data.lightMatrix = s_data.lightProjection * s_data.lightView;*/

            directionalLights++;
        }
        else if (dynamic_cast<const SkyLight*>(light))
        {
            light->setShaderUniforms(shader, 0);
        }
        
    }

    shader->setInt("uNumPointLights", pointLights);
    shader->setInt("uUsingDirectionalLight", directionalLights); // TODO: multiple directional lights
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

void Renderer3D::setEnvironment(const Reference<EnvironmentMap>& environment)
{
    s_data.environment = environment;
}

}
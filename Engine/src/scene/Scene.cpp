#include <scene/Scene.h>
#include <renderer/Renderer2D.h>
#include <maths/matrix/matrix_func.h>
#include <scene/SceneEntity.h>
#include <renderer/Renderer3D.h>
#include <renderer/MeshFactory.h>
#include <scene/Components.h>

Scene::Scene()
{

}

Scene::~Scene()
{
    
}

void Scene::onUpdateEditor(float dt, EditorCamera& camera)
{
    LightSetup setup;
    {
        auto view = m_registry.view<SkyLightComponent>();
        for (auto& entity : view)
            setup.setSkylight(view.get<SkyLightComponent>(entity).intensity);
        
        view = m_registry.view<DirectionalLightComponent, TransformComponent>();
        for (auto& entity : view)
        {
            auto& dirLight = view.get<DirectionalLightComponent>(entity);
            auto& transform = view.get<TransformComponent>(entity);
            setup.setDirectionalLight({ transform.rotation, dirLight.radiance, dirLight.intensity });
        }

        std::vector<PointLight> pointLights;
        view = m_registry.view<PointLightComponent, TransformComponent>();
        for (auto& entity : view)
        {
            auto& pointLight = view.get<PointLightComponent>(entity);
            auto& transform = view.get<TransformComponent>(entity);
            pointLights.push_back({ transform.translation, pointLight.radiance, pointLight.intensity });
        }

        setup.setPointLights(pointLights);
    }

    Renderer3D::setLights(setup);

    Renderer3D::beginScene(camera);

    auto view = m_registry.view<MeshComponent, TransformComponent>();
    for (auto& entityID : view)
    {
        SceneEntity entity = { entityID, this };
        if (!entity.getComponent<MeshComponent>().mesh)
        {
            continue;
        }
        Renderer3D::submit(entity.getComponent<MeshComponent>().mesh, entity.getComponent<TransformComponent>().getTransform());
    }

    Renderer3D::endScene();

    Renderer2D::beginScene(camera);
    this->render2DEntities();

    Renderer2D::endScene();
}

void Scene::render2DEntities()
{
    auto view = m_registry.view<SpriteRendererComponent, TransformComponent>();
    for (auto& entity : view)
    {
        auto& sprite = view.get<SpriteRendererComponent>(entity);
        if (sprite.texture != nullptr)
        {
            if (sprite.usingTexRect)
            {
                Renderer2D::renderSprite(sprite.texture, view.get<TransformComponent>(entity).getTransform(), sprite.textureRect);
            }
            else
            {
                Renderer2D::renderSprite(sprite.texture, view.get<TransformComponent>(entity).getTransform());
            }
        }
        else
        {
            Renderer2D::renderQuad(view.get<TransformComponent>(entity).getTransform(), sprite.color);
        }
    }
}

SceneEntity Scene::createEntity(const std::string& name)
{
    auto entity = m_registry.create();
    m_registry.emplace<TagComponent>(entity, name);
    return SceneEntity(entity, this);
}

void Scene::destroyEntity(SceneEntity& entity)
{
    m_registry.destroy(entity.m_entityHandle);
    entity.m_entityHandle = nullptr;
}

void Scene::onUpdateRuntime(float dt)
{
    // Game logic updating
    auto colliders = m_registry.view<BoxCollider2DComponent, NativeScriptComponent>();
    for (auto& entityID : colliders)
    {
        SceneEntity entity = { entityID, this };
        for (auto& otherID : colliders)
        {
            SceneEntity other = { otherID, this };
            if (entity == other)
                continue;

            if (math::intersects(entity.getComponent<BoxCollider2DComponent>().box, other.getComponent<BoxCollider2DComponent>().box))
            {
                if (entity.getComponent<NativeScriptComponent>().instance)
                    entity.getComponent<NativeScriptComponent>().instance->onCollide2D();

                if (other.getComponent<NativeScriptComponent>().instance)
                    other.getComponent<NativeScriptComponent>().instance->onCollide2D();
            }
        }
    }

    auto view = m_registry.view<NativeScriptComponent>();
    for (auto& entity : view)
    {
        auto& script = view.get<NativeScriptComponent>(entity);

        if (!script.instance)
        {
            script.instance = script.instantiateScript();
            script.instance->m_entity = SceneEntity(entity, this);
            script.instance->onCreate();
        }

        script.instance->onUpdate(dt);
    }

    // Rendering
    Camera* camera = nullptr;
    math::mat4 transform;

    {
        auto view = m_registry.view<TransformComponent, CameraComponent>();
        for (auto& entityID : view)
        {
            SceneEntity entity = { entityID, this };
            if (entity.getComponent<CameraComponent>().primary)
            {
                camera = &(entity.getComponent<CameraComponent>().camera);
                transform = entity.getComponent<TransformComponent>().getTransform();
                break;
            }
        }
    }

    if (camera != nullptr)
    {
        Renderer2D::beginScene(*camera, transform);

        this->render2DEntities();

        Renderer2D::endScene();
    }
}

SceneEntity Scene::getPrimaryCameraEntity()
{
    auto view = m_registry.view<CameraComponent>();
    for (auto& entity : view)
    {
        auto& camera = view.get<CameraComponent>(entity);
        if (camera.primary)
        {
            return SceneEntity(entity, this);
        }
    }

    return {};
}

void Scene::onViewportResize(uint32_t width, uint32_t height)
{
    m_viewportWidth = width;
    m_viewportHeight = height;

    auto view = m_registry.view<CameraComponent>();
    for (auto& entity : view)
    {
        auto& cameraComponent = view.get<CameraComponent>(entity);
        cameraComponent.camera.setViewportSize(width, height);
    }
}

template<>
void Scene::onComponentAdded<CameraComponent>(SceneEntity& entity, CameraComponent& component)
{
    component.camera.setViewportSize(m_viewportWidth, m_viewportHeight);
}

template<> void Scene::onComponentAdded<TransformComponent>(SceneEntity& entity, TransformComponent& component) {}
template<> void Scene::onComponentAdded<SpriteRendererComponent>(SceneEntity& entity, SpriteRendererComponent& component) {}
template<> void Scene::onComponentAdded<BoxCollider2DComponent>(SceneEntity& entity, BoxCollider2DComponent& component) {}
template<> void Scene::onComponentAdded<NativeScriptComponent>(SceneEntity& entity, NativeScriptComponent& component) {}
template<> void Scene::onComponentAdded<TagComponent>(SceneEntity& entity, TagComponent& component) {}
template<> void Scene::onComponentAdded<TextRendererComponent>(SceneEntity& entity, TextRendererComponent& component) {}
template<> void Scene::onComponentAdded<MeshComponent>(SceneEntity& entity, MeshComponent& component) {}
template<> void Scene::onComponentAdded<SkyLightComponent>(SceneEntity& entity, SkyLightComponent& component) {}
template<> void Scene::onComponentAdded<PointLightComponent>(SceneEntity& entity, PointLightComponent& component) {}
template<> void Scene::onComponentAdded<DirectionalLightComponent>(SceneEntity& entity, DirectionalLightComponent& component) {}
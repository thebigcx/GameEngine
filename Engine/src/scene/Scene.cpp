#include <scene/Scene.h>
#include <scene/Components.h>
#include <renderer/Renderer2D.h>
#include <maths/matrix/matrix_func.h>
#include <scene/SceneEntity.h>

Scene::Scene()
{

}

Scene::~Scene()
{
    
}

void Scene::onUpdateEditor(float dt, EditorCamera& camera)
{
    //Renderer2D::beginScene(camera);
    OrthographicCamera cam;
    Renderer2D::beginScene(cam);

    auto view = m_registry.view<SpriteRendererComponent, TransformComponent>();
    for (auto& entity : view)
    {
        Renderer2D::renderQuad(view.get<TransformComponent>(entity).getTransform(), view.get<SpriteRendererComponent>(entity).color);
    }

    Renderer2D::endScene();
}

SceneEntity Scene::createEntity()
{
    auto entity = m_registry.create();
    return SceneEntity(entity, this);
}

void Scene::destroyEntity(SceneEntity& entity)
{
    m_registry.destroy(entity.m_entityHandle);
    entity.m_entityHandle = nullptr;
}

void Scene::onUpdateRuntime(float dt)
{
    /*m_registry.view<NativeScriptComponent>().each<NativeScriptComponent>([&](auto& entity, NativeScriptComponent& script)
    {
        if (!script.instance)
        {
            script.instance = script.instantiateScript();
            script.instance->m_entity = entity;
            script.instance->onCreate();
        }

        script.instance->onUpdate(dt);
    });*/
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

    Camera* camera = nullptr;
    math::mat4 transform;

    {
        auto view = m_registry.view<TransformComponent, CameraComponent>();
        for (auto& entity : view)
        {
            if (entity->getRegistry()->get<CameraComponent>(entity).primary)
            {
                camera = &(entity->getRegistry()->get<CameraComponent>(entity).camera);
                transform = entity->getRegistry()->get<TransformComponent>(entity).getTransform();
                break;
            }
        }
    }

    if (camera != nullptr)
    {
        Renderer2D::beginScene(*camera, transform);

        auto view = m_registry.view<SpriteRendererComponent, TransformComponent>();
        for (auto& entity : view)
        {
            Renderer2D::renderQuad(view.get<TransformComponent>(entity).getTransform(), view.get<SpriteRendererComponent>(entity).color);
        }

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

template<typename T>
void Scene::onComponentAdded(SceneEntity& entity, T& component)
{
    Logger::getCoreLogger()->error("Unknown component type: %s", typeid(T).name());
}

template<>
void Scene::onComponentAdded<CameraComponent>(SceneEntity& entity, CameraComponent& component)
{
    component.camera.setViewportSize(m_viewportWidth, m_viewportHeight);
}

template<>
void Scene::onComponentAdded<TransformComponent>(SceneEntity& entity, TransformComponent& component)
{
    
}

template<>
void Scene::onComponentAdded<SpriteRendererComponent>(SceneEntity& entity, SpriteRendererComponent& component)
{
    
}
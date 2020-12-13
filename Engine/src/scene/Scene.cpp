#include <scene/Scene.h>
#include <scene/Components.h>
#include <renderer/Renderer2D.h>

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

Entity* Scene::getPrimaryCameraEntity()
{
    auto view = m_registry.view<CameraComponent>();
    for (auto& entity : view)
    {
        auto& camera = view.get<CameraComponent>(entity);
        if (camera.primary)
        {
            return entity;
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
void Scene::onComponentAdded(Entity* entity, T& component)
{
    Logger::getCoreLogger()->error("Unknown component type: %s", typeid(T).name());
}

template<>
void Scene::onComponentAdded<CameraComponent>(Entity* entity, CameraComponent& component)
{
    component.camera.setViewportSize(m_viewportWidth, m_viewportHeight);
}
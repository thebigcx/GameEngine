#include <scene/Scene.h>
#include <renderer/Renderer2D.h>
#include <maths/matrix/matrix_func.h>

#include <renderer/Renderer3D.h>
#include <renderer/MeshFactory.h>
#include <scene/Components.h>
#include <renderer/Assets.h>
#include <util/Timer.h>

namespace Engine
{

Scene::Scene()
{
    
}

Scene::~Scene()
{
    
}

void Scene::onUpdateEditor(float dt, EditorCamera& camera)
{
    Assets::get<Shader>("pbr")->bind();
    Renderer3D::clearLights();
    {
        auto skylights = m_rootObject.getChildrenWithComponent<SkyLightComponent>();
        for (auto& object : skylights)
        {   
            auto light = object->getComponent<SkyLightComponent>();
            auto skyLight = new SkyLight(light.radiance, light.intensity);
            skyLight->addToRenderer();
        }
        
        auto directionalLights = m_rootObject.getChildrenWithComponents<DirectionalLightComponent, TransformComponent>();
        for (auto& object : directionalLights)
        {
            auto& light = object->getComponent<DirectionalLightComponent>();
            auto& transform = object->getComponent<TransformComponent>();

            auto dirLight = new DirectionalLight(light.radiance, light.intensity, transform.rotation);
            dirLight->addToRenderer();
        }

        auto pointLightObjects = m_rootObject.getChildrenWithComponents<PointLightComponent, TransformComponent>();
        for (auto& object : pointLightObjects)
        {
            auto& light = object->getComponent<PointLightComponent>();
            auto& transform = object->getComponent<TransformComponent>();

            auto pointLight = new PointLight(light.radiance, light.intensity, transform.translation, light.attenuation);
            pointLight->addToRenderer();
        }
    }

    Renderer3D::beginScene(camera);

    this->render3DEntities();

    Renderer3D::endScene();

    Renderer2D::beginScene(camera);
    this->render2DEntities();

    Renderer2D::endScene();
}

void recurseRender(GameObject& object)
{
    if (object.hasComponents<MeshComponent, TransformComponent, MeshRendererComponent>())
    {
        auto& mesh = object.getComponent<MeshComponent>().mesh;
        auto transform = object.getComponent<TransformComponent>().getTransform();

        if (object.getComponent<MeshRendererComponent>().materials.size() > 0)
        {
            auto& material = object.getComponent<MeshRendererComponent>().materials.at(0);

            if (material && mesh)
            {
                Renderer3D::submit(mesh, transform, material);
            }
        }
    }

    for (auto& child : object.getChildren())
    {
        recurseRender(child);
    }
}

void Scene::render3DEntities()
{
    auto objects = m_rootObject.getChildren();

    for (auto& object : objects)
    {
        recurseRender(object);
    }
}

void Scene::render2DEntities()
{
    auto renderables = m_rootObject.getChildrenWithComponents<TransformComponent, SpriteRendererComponent>();
    for (auto& object : renderables)
    {
        auto& sprite = object->getComponent<SpriteRendererComponent>();
        if (sprite.texture != nullptr)
        {
            if (sprite.usingTexRect)
            {
                Renderer2D::renderSprite(sprite.texture, object->getComponent<TransformComponent>().getTransform(), sprite.textureRect);
            }
            else
            {
                Renderer2D::renderSprite(sprite.texture, object->getComponent<TransformComponent>().getTransform());
            }
        }
        else
        {
            Renderer2D::renderQuad(object->getComponent<TransformComponent>().getTransform(), sprite.color);
        }
    }
}

GameObject* Scene::createGameObject(const std::string& name)
{
    auto object = m_rootObject.addChild();
    object->addComponent<TagComponent>(name);
    return object;
}

void Scene::onUpdateRuntime(float dt)
{
    // Game logic updating
    /*auto colliders = m_rootObject.getChildrenWithComponents<BoxCollider2DComponent, NativeScriptComponent>();
    for (auto& object1 : colliders)
    {
        for (auto& object2 : colliders)
        {
            if (object1 == object2)
                continue;

            if (math::intersects(object1->getComponent<BoxCollider2DComponent>().box, object2->getComponent<BoxCollider2DComponent>().box))
            {
                if (object1->getComponent<NativeScriptComponent>().instance)
                    object1->getComponent<NativeScriptComponent>().instance->onCollide2D();

                if (object2->getComponent<NativeScriptComponent>().instance)
                    object2->getComponent<NativeScriptComponent>().instance->onCollide2D();
            }
        }
    }*/
    // TODO: replace with rigid bodies

    auto scripts = m_rootObject.getChildrenWithComponent<NativeScriptComponent>();
    for (auto& object : scripts)
    {
        auto& script = object->getComponent<NativeScriptComponent>();

        if (!script.instance)
        {
            script.instance = script.instantiateScript();
            script.instance->m_gameObject = object;
            script.instance->onCreate();
        }

        script.instance->onUpdate(dt);
    }

    // Lua scripts
    auto luaScripts = m_rootObject.getChildrenWithComponent<LuaScriptComponent>();
    for (auto& object : luaScripts)
    {
        auto& script = object->getComponent<LuaScriptComponent>();
        script.scriptEngine.setScript(script.filePath); // TODO: possibly not do this every frame (50-100microseconds)

        if (!script.initialized)
        {
            script.scriptEngine.onStart();
            script.initialized = true;
        }
        
        script.scriptEngine.onUpdate(dt);
    }

    // Rendering
    Camera* camera = nullptr;
    math::mat4 transform;

    {
        auto cameras = m_rootObject.getChildrenWithComponents<TransformComponent, CameraComponent>();
        for (auto& object : cameras)
        {
            if (object->getComponent<CameraComponent>().primary)
            {
                camera = &(object->getComponent<CameraComponent>().camera);
                transform = object->getComponent<TransformComponent>().getTransform();
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

GameObject* Scene::getPrimaryCameraGameObject()
{
    auto cameras = m_rootObject.getChildrenWithComponent<CameraComponent>();
    for (auto& object : cameras)
    {
        auto& camera = object->getComponent<CameraComponent>();
        if (camera.primary)
        {
            return object;
        }
    }

    return nullptr;
}

void Scene::onViewportResize(uint32_t width, uint32_t height)
{
    m_viewportWidth = width;
    m_viewportHeight = height;

    auto cameras = m_rootObject.getChildrenWithComponent<CameraComponent>();
    for (auto& object : cameras)
    {
        auto& cameraComponent = object->getComponent<CameraComponent>();
        cameraComponent.camera.setViewportSize(width, height);
    }
}

template<>
void Scene::onComponentAdded<CameraComponent>(GameObject& object, CameraComponent& component)
{
    component.camera.setViewportSize(m_viewportWidth, m_viewportHeight); // TODO: better way
}

template<> void Scene::onComponentAdded<TransformComponent>(GameObject& object, TransformComponent& component) {}
template<> void Scene::onComponentAdded<SpriteRendererComponent>(GameObject& object, SpriteRendererComponent& component) {}
template<> void Scene::onComponentAdded<BoxCollider2DComponent>(GameObject& object, BoxCollider2DComponent& component) {}
template<> void Scene::onComponentAdded<NativeScriptComponent>(GameObject& object, NativeScriptComponent& component) {}
template<> void Scene::onComponentAdded<TagComponent>(GameObject& object, TagComponent& component) {}
template<> void Scene::onComponentAdded<TextRendererComponent>(GameObject& object, TextRendererComponent& component) {}
template<> void Scene::onComponentAdded<MeshComponent>(GameObject& object, MeshComponent& component) {}
template<> void Scene::onComponentAdded<SkyLightComponent>(GameObject& object, SkyLightComponent& component) {}
template<> void Scene::onComponentAdded<PointLightComponent>(GameObject& object, PointLightComponent& component) {}
template<> void Scene::onComponentAdded<DirectionalLightComponent>(GameObject& object, DirectionalLightComponent& component) {}
template<> void Scene::onComponentAdded<MeshRendererComponent>(GameObject& object, MeshRendererComponent& component) {}
template<> void Scene::onComponentAdded<LuaScriptComponent>(GameObject& object, LuaScriptComponent& component) {}

}
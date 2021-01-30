#include <scene/Scene.h>
#include <renderer/Renderer2D.h>
#include <maths/matrix/matrix_func.h>
#include <renderer/Renderer3D.h>
#include <scene/Components.h>
#include <script/ScriptController.h>

namespace Engine
{

Scene::Scene()
{
    
}

Scene::~Scene()
{

}

Reference<Scene> Scene::create()
{
    auto scene = Reference<Scene>(new Scene());
    /*auto cam = scene->createGameObject("Main Camera");
    auto& cameraComp = cam->addComponent<CameraComponent>();
    cameraComp.primary = true;
    cameraComp.camera.setProjectionType(ProjectionType::Perspective);
    cam->addComponent<TransformComponent>();*/

    return scene;
}

void Scene::onUpdateEditor(float dt, EditorCamera& camera)
{
    this->setLights();

    Renderer3D::beginScene(camera);

    this->render3DEntities();

    Renderer3D::endScene();

    Renderer2D::beginScene(camera);
    this->render2DEntities();

    Renderer2D::endScene();
}

void Scene::recurseRender2D(GameObject& object)
{
    if (object.hasComponents<TransformComponent, SpriteRendererComponent>())
    {
        auto& sprite = object.getComponent<SpriteRendererComponent>();
        auto transform = object.getComponent<TransformComponent>().getTransform();

        math::frect textureRect = sprite.usingTexRect ? sprite.textureRect : math::frect(0, 0, sprite.texture->getWidth(), sprite.texture->getHeight());

        Renderer2D::renderSprite(sprite.texture, transform, textureRect);
    }

    for (auto& child : object.getChildren())
    {
        recurseRender2D(child);
    }
}

void Scene::recurseRender3D(GameObject& object)
{
    if (object.hasComponents<MeshComponent, TransformComponent, MeshRendererComponent>())
    {
        auto& mesh = object.getComponent<MeshComponent>().mesh;
        auto transform = object.getComponent<TransformComponent>().getTransform();
        auto& material = object.getComponent<MeshRendererComponent>().material;

        if (material && mesh)
        {
            Renderer3D::submit(mesh, transform, material);
        }
    }

    for (auto& child : object.getChildren())
    {
        recurseRender3D(child);
    }
}

void Scene::render3DEntities()
{
    auto objects = m_rootObject.getChildren();

    for (auto& object : objects)
    {
        recurseRender3D(object);
    }
}

void Scene::render2DEntities()
{
    auto renderables = m_rootObject.getChildren();

    for (auto& object : renderables)
    {
        recurseRender2D(object);
    }
}

void Scene::setLights()
{
    Renderer3D::clearLights();
    {
        auto skylights = m_rootObject.getChildrenWithComponent<SkyLightComponent>();
        for (auto& object : skylights)
        {   
            auto& light = object->getComponent<SkyLightComponent>();

            Renderer3D::addLight(&light.light);
        }
        
        auto directionalLights = m_rootObject.getChildrenWithComponents<DirectionalLightComponent, TransformComponent>();
        for (auto& object : directionalLights)
        {
            auto& light = object->getComponent<DirectionalLightComponent>();
            auto& transform = object->getComponent<TransformComponent>();

            math::quat rotation = math::quat(math::radians(transform.rotation));
            light.light.direction = rotation * math::vec3(0, -1, 0);

            Renderer3D::addLight(&light.light);
        }

        auto pointLightObjects = m_rootObject.getChildrenWithComponents<PointLightComponent, TransformComponent>();
        for (auto& object : pointLightObjects)
        {
            auto& light = object->getComponent<PointLightComponent>();
            auto& transform = object->getComponent<TransformComponent>();

            light.light.position = transform.translation;

            Renderer3D::addLight(&light.light);
        }
    }
}

GameObject* Scene::createGameObject(const std::string& name)
{
    auto object = m_rootObject.addChild();
    object->addComponent<TagComponent>(name);
    return object;
}

void Scene::onScenePlay()
{
    auto csscripts = m_rootObject.getChildrenWithComponent<CSharpScriptComponent>();
    for (auto& object : csscripts)
    {
        auto& script = object->getComponent<CSharpScriptComponent>();
        script.script = ScriptController::getInstance()->loadScript(script.filepath);
    }
}

void Scene::onUpdateRuntime(float dt)
{
    // Native scripts
    auto scripts = m_rootObject.getChildrenWithComponent<NativeScriptComponent>();
    for (auto& object : scripts)
    {
        auto& script = object->getComponent<NativeScriptComponent>();

        if (!script.instance)
        {
            script.instance = script.instantiateScript();
            script.instance->m_gameObject = object;
            script.instance->onStart();
        }

        script.instance->onUpdate(dt);
    }

    // C# scripts
    auto csscripts = m_rootObject.getChildrenWithComponent<CSharpScriptComponent>();
    for (auto& object : csscripts)
    {
        auto& script = object->getComponent<CSharpScriptComponent>();

        if (!script.initialized)
        {
            script.script->onStart();
            script.initialized = true;
        }

        script.script->onUpdate(dt);
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
        this->setLights();

        Renderer3D::beginScene(*camera, transform);

        this->render3DEntities();

        Renderer3D::endScene();

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
template<> void Scene::onComponentAdded<NativeScriptComponent>(GameObject& object, NativeScriptComponent& component) {}
template<> void Scene::onComponentAdded<TagComponent>(GameObject& object, TagComponent& component) {}
template<> void Scene::onComponentAdded<TextRendererComponent>(GameObject& object, TextRendererComponent& component) {}
template<> void Scene::onComponentAdded<MeshComponent>(GameObject& object, MeshComponent& component) {}
template<> void Scene::onComponentAdded<SkyLightComponent>(GameObject& object, SkyLightComponent& component) {}
template<> void Scene::onComponentAdded<PointLightComponent>(GameObject& object, PointLightComponent& component) {}
template<> void Scene::onComponentAdded<DirectionalLightComponent>(GameObject& object, DirectionalLightComponent& component) {}
template<> void Scene::onComponentAdded<MeshRendererComponent>(GameObject& object, MeshRendererComponent& component) {}
template<> void Scene::onComponentAdded<CSharpScriptComponent>(GameObject& object, CSharpScriptComponent& component) {}
template<> void Scene::onComponentAdded<BoxCollider2DComponent>(GameObject& object, BoxCollider2DComponent& component) {}
template<> void Scene::onComponentAdded<RigidBody2DComponent>(GameObject& object, RigidBody2DComponent& component) {}

}
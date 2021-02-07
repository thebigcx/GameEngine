#include <scene/Scene.h>
#include <renderer/Renderer2D.h>
#include <maths/matrix/matrix_func.h>
#include <renderer/Renderer3D.h>
#include <scene/Components.h>
#include <scene/NativeScript.h>
#include <script/ScriptController.h>
#include <scene/SceneCamera.h>
#include <util/Timer.h>

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
    if (object.hasComponents<Transform, SpriteRendererComponent>())
    {
        auto& sprite = object.getComponent<SpriteRendererComponent>();
        auto transform = object.getComponent<Transform>().worldMatrix();

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
    if (object.hasComponents<MeshComponent, Transform, MeshRendererComponent>())
    {
        auto& mesh = object.getComponent<MeshComponent>().mesh;
        auto transform = object.getComponent<Transform>().worldMatrix();
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
        auto skylights = m_rootObject.getChildrenWithComponent<SkyLight>();
        for (auto& object : skylights)
        {   
            auto& light = object->getComponent<SkyLight>();

            Renderer3D::addLight(&light);
        }
        
        auto directionalLights = m_rootObject.getChildrenWithComponents<DirectionalLight, Transform>();
        for (auto& object : directionalLights)
        {
            auto& light = object->getComponent<DirectionalLight>();

            Renderer3D::addLight(&light);
        }

        auto pointLightObjects = m_rootObject.getChildrenWithComponents<PointLight, Transform>();
        for (auto& object : pointLightObjects)
        {
            auto& light = object->getComponent<PointLight>();

            Renderer3D::addLight(&light);
        }
    }
}

GameObject* Scene::createGameObject(const std::string& name)
{
    auto object = m_rootObject.createChild();
    object->createComponent<Tag>(name);
    return object;
}

void Scene::onScenePlay()
{
    auto csscripts = m_rootObject.getChildrenWithComponent<ScriptInstance>();
    for (auto& object : csscripts)
    {
        auto& script = object->getComponent<ScriptInstance>();
        //script.setScript(ScriptController::getInstance()->loadScript(script.getScript()->getPath()));
    }
}

void Scene::onUpdateRuntime(float dt)
{
    // Native scripts
    auto scripts = m_rootObject.getChildrenWithComponent<NativeScript>();
    for (auto& object : scripts)
    {
        auto& script = object->getComponent<NativeScript>();

        if (!script.instance)
        {
            script.instance = script.instantiateScript();
            script.instance->m_gameObject = object;
            script.instance->onStart();
        }

        script.instance->onUpdate(dt);
    }

    // C# scripts
    auto csscripts = m_rootObject.getChildrenWithComponent<ScriptInstance>();
    for (auto& object : csscripts)
    {
        auto& script = object->getComponent<ScriptInstance>();

        /*if (!script.initialized)
        {
            script.script->onStart();
            script.initialized = true;
        }*/

        script.getScript()->onUpdate(dt);
    }

    // Rendering
    Camera* camera = nullptr;
    math::mat4 transform;

    {
        auto cameras = m_rootObject.getChildrenWithComponents<Transform, SceneCamera>();
        for (auto& object : cameras)
        {
            if (object->getComponent<SceneCamera>().primary)
            {
                camera = &object->getComponent<SceneCamera>();
                transform = object->getComponent<Transform>().worldMatrix();
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
    auto cameras = m_rootObject.getChildrenWithComponent<SceneCamera>();
    for (auto& object : cameras)
    {
        auto& camera = object->getComponent<SceneCamera>();
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

    auto cameras = m_rootObject.getChildrenWithComponent<SceneCamera>();
    for (auto& object : cameras)
    {
        auto& camera = object->getComponent<SceneCamera>();
        camera.setViewportSize(width, height);
    }
}

void Scene::onComponentAdded(GameObject& object, GameComponent* component)
{
    if (dynamic_cast<SceneCamera*>(component))
    {
        static_cast<SceneCamera*>(component)->setViewportSize(m_viewportWidth, m_viewportHeight);
    }
}

}
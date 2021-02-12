#include <scene/Scene.h>
#include <renderer/Renderer2D.h>
#include <maths/matrix/matrix_func.h>
#include <renderer/Renderer3D.h>
#include <scene/Components.h>
#include <scene/NativeScript.h>
#include <script/ScriptController.h>
#include <scene/SceneCamera.h>
#include <util/Timer.h>
#include <audio/AudioSource.h>
#include <physics/2D/PhysicsWorld2D.h>

namespace Engine
{

Scene::Scene()
{
    
}

Scene::~Scene()
{
    for (auto child : m_rootObject.getChildren())
    {
        m_rootObject.removeChild(child);
    }
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

void Scene::recurseRender2D(GameObject* object)
{
    if (object->hasComponents<Transform, SpriteRendererComponent>())
    {
        auto sprite = object->getComponent<SpriteRendererComponent>();
        auto transform = object->getComponent<Transform>()->worldMatrix();

        math::frect textureRect = sprite->usingTexRect ? sprite->textureRect : math::frect(0, 0, sprite->texture->getWidth(), sprite->texture->getHeight());

        Renderer2D::renderSprite(sprite->texture, transform, textureRect);
    }

    for (auto& child : object->getChildren())
    {
        recurseRender2D(child);
    }
}

void Scene::recurseRender3D(GameObject* object)
{
    if (object->hasComponents<MeshComponent, Transform, MeshRendererComponent>())
    {
        auto& mesh = object->getComponent<MeshComponent>()->mesh;
        auto transform = object->getComponent<Transform>()->worldMatrix();
        auto& material = object->getComponent<MeshRendererComponent>()->material;

        if (material && mesh)
        {
            Renderer3D::submit(mesh, transform, material);
        }
    }

    for (auto& child : object->getChildren())
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
            auto light = object->getComponent<SkyLight>();

            Renderer3D::addLight(light);
        }
        
        auto directionalLights = m_rootObject.getChildrenWithComponents<DirectionalLight, Transform>();
        for (auto& object : directionalLights)
        {
            auto light = object->getComponent<DirectionalLight>();

            Renderer3D::addLight(light);
        }

        auto pointLightObjects = m_rootObject.getChildrenWithComponents<PointLight, Transform>();
        for (auto& object : pointLightObjects)
        {
            auto light = object->getComponent<PointLight>();

            Renderer3D::addLight(light);
        }
    }
}

GameObject* Scene::createGameObject(const std::string& name)
{
    auto object = m_rootObject.createChild();
    object->createComponent<Tag>(name);
    return object;
}

void Scene::onSceneStart()
{
    auto csscripts = m_rootObject.getChildrenWithComponent<ScriptInstance>();
    for (auto& object : csscripts)
    {
        auto script = object->getComponent<ScriptInstance>();
        //script.setScript(ScriptController::getInstance()->loadScript(script.getScript()->getPath()));
    }

    auto sources = m_rootObject.getChildrenWithComponentsRecursive<AudioSource, Transform>();
    for (auto& object : sources)
    {
        auto source = object->getComponent<AudioSource>();

        if (source->playOnStart)
            source->play();
    }
}

void Scene::onSceneFinish()
{
    auto sources = m_rootObject.getChildrenWithComponentsRecursive<AudioSource, Transform>();
    for (auto& object : sources)
    {
        if (object->getComponent<AudioSource>()->getState() == AudioSource::State::Playing)
            object->getComponent<AudioSource>()->stop();
    }
}

void Scene::onUpdateRuntime(float dt)
{
    // Native scripts
    auto scripts = m_rootObject.getChildrenWithComponent<NativeScript>();
    for (auto& object : scripts)
    {
        auto script = object->getComponent<NativeScript>();

        if (!script->instance)
        {
            script->instance = script->instantiateScript();
            script->instance->m_gameObject = object;
            script->instance->onStart();
        }

        script->instance->onUpdate(dt);
    }

    // C# scripts
    auto csscripts = m_rootObject.getChildrenWithComponent<ScriptInstance>();
    for (auto& object : csscripts)
    {
        object->getComponent<ScriptInstance>()->getScript()->onUpdate(dt);
    }

    // Rendering
    Camera* camera = nullptr;
    math::mat4 transform;

    auto cameraObj = getPrimaryCameraGameObject();
    if (cameraObj)
    {
        camera = cameraObj->getComponent<SceneCamera>();
        transform = cameraObj->getComponent<Transform>()->worldMatrix();
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
    auto cameras = m_rootObject.getChildrenWithComponentRecursive<SceneCamera>();
    for (auto& object : cameras)
    {
        auto camera = object->getComponent<SceneCamera>();
        if (camera->primary)
        {
            return object;
        }
    }

    return nullptr;
}

GameObject* Scene::getPhysicsWorld2D()
{
    auto worlds = m_rootObject.getChildrenWithComponentRecursive<PhysicsWorld2D>();
    return worlds.size() > 0 ? worlds[0] : nullptr;
}

void Scene::onViewportResize(uint32_t width, uint32_t height)
{
    m_viewportWidth = width;
    m_viewportHeight = height;

    auto cameras = m_rootObject.getChildrenWithComponent<SceneCamera>();
    for (auto& object : cameras)
    {
        auto camera = object->getComponent<SceneCamera>();
        camera->setViewportSize(width, height);
    }
}

}
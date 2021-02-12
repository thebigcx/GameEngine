#pragma once

#include <vector>

#include <core/Core.h>
#include <scene/GameObject.h>
#include <maths/math.h>

namespace Engine
{

class EditorCamera;

class Scene
{
private:
    Scene();

public:
    ~Scene();

    static Reference<Scene> create();

    math::vec2 getViewportSize() const { return { m_viewportWidth, m_viewportHeight }; }
    
    void onUpdateEditor(float dt, EditorCamera& camera);
    void onUpdateRuntime(float dt);

    void onViewportResize(uint32_t width, uint32_t height);

    GameObject* createGameObject(const std::string& name);

    GameObject& getRootGameObject()
    {
        return m_rootObject;
    }

    // Returns singleton components/objects
    // TODO: SingletonComponent inherit GameComponent. Scene::getPrimarySingletonComponent<T>()
    GameObject* getPrimaryCameraGameObject();
    GameObject* getPhysicsWorld2D();
    // TODO: root object is available to add components to. such as physics world 2d.

    void onSceneStart();
    void onSceneFinish();

    const std::string& getPath() const
    {
        return m_path;
    }

    void setPath(const std::string& path)
    {
        m_path = path;
    }

private:
    GameObject m_rootObject;

    uint32_t m_viewportWidth = 0, m_viewportHeight = 0;

    std::string m_path = "";

    void render2DEntities();
    void render3DEntities();

    void recurseRender3D(GameObject* object);
    void recurseRender2D(GameObject* object);

    void setLights();
};

}
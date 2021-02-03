#pragma once

#include <vector>

#include <core/Core.h>
#include <scene/GameObject.h>

namespace Engine
{

class EditorCamera;

class Scene
{
    friend class SceneSerializer;

private:
    Scene();

public:
    ~Scene();

    static Reference<Scene> create();
    
    void onUpdateEditor(float dt, EditorCamera& camera);
    void onUpdateRuntime(float dt);

    void onViewportResize(uint32_t width, uint32_t height);

    GameObject* createGameObject(const std::string& name);

    GameObject& getRootGameObject()
    {
        return m_rootObject;
    }

    GameObject* getPrimaryCameraGameObject();
    
    void onComponentAdded(GameObject& object, GameComponent* component);

    void onScenePlay();

private:
    GameObject m_rootObject;

    uint32_t m_viewportWidth = 0, m_viewportHeight = 0;

    void render2DEntities();
    void render3DEntities();

    void recurseRender3D(GameObject& object);
    void recurseRender2D(GameObject& object);

    void setLights();
};

}
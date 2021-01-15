#pragma once

#include <vector>

#include <scene/GameObject.h>
#include <scene/EditorCamera.h>
#include <renderer/Material.h>
#include <renderer/Lighting.h>
#include <script/ScriptEngine.h>

namespace Engine
{

class Scene
{
public:
    Scene();
    ~Scene();
    
    void onUpdateEditor(float dt, EditorCamera& camera);
    void onUpdateRuntime(float dt);

    void onViewportResize(uint32_t width, uint32_t height);

    GameObject* createGameObject(const std::string& name);

    GameObject& getRootGameObject()
    {
        return m_rootObject;
    }

    GameObject* getPrimaryCameraGameObject();
    
    template<typename T>
    void onComponentAdded(GameObject& object, T& component);

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
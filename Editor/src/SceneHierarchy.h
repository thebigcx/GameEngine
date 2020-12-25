#pragma once

#include <scene/Scene.h>
#include <core/Core.h>
#include <scene/SceneEntity.h>
#include <renderer/Framebuffer.h>

#include "FileSelectWindow.h"

namespace Engine
{

class SceneHierarchy
{
public:
    SceneHierarchy();
    SceneHierarchy(const Shared<Scene>& scene);

    template<typename T, typename F>
    void drawComponent(const std::string& name, GameObject& entity, const F& func);

    void drawProperties(GameObject& entity);
    
    void setContext(const Shared<Scene>& context)
    {
        m_context = context;
    }

    void recurseTree(GameObject& entity);

    void onImGuiRender();

    void drawSceneRenderer();

    GameObject* getSelectedEntity()
    {
        return m_selection;
    }

private:
    Shared<Scene> m_context;

    GameObject* m_selection = nullptr;
    GameObject* m_deletedEntity = nullptr;

    void textureSelect(Shared<Texture2D>& texture);
};

}
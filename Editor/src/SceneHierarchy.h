#pragma once

#include <scene/Scene.h>
#include <core/Core.h>

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
    void drawComponent(const std::string& name, GameObject& object, const F& func);

    void drawProperties(GameObject& object);
    
    void setContext(const Shared<Scene>& context)
    {
        m_context = context;
    }

    void recurseTree(GameObject& object);

    void onImGuiRender();

    void drawSceneRenderer();

    GameObject* getSelectedGameObject()
    {
        return m_selection;
    }

private:
    Shared<Scene> m_context;

    GameObject* m_selection = nullptr;
    GameObject* m_deletedGameObject = nullptr;

    void textureSelect(Shared<Texture2D>& texture);
};

}
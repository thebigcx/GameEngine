#pragma once

#include <scene/Scene.h>
#include <core/Core.h>

#include <renderer/Framebuffer.h>

#include "../FileDialog.h"

namespace Engine
{

class SceneHierarchyPanel
{
public:
    SceneHierarchyPanel();
    SceneHierarchyPanel(Scene* scene);

    template<typename T, typename F>
    void drawComponent(const std::string& name, GameObject& object, const F& func);

    void drawProperties(GameObject& object);
    
    void setContext(Scene* context) { m_context = context; }

    void recurseTree(GameObject& object);

    void onImGuiRender();

    GameObject* getSelectedGameObject()
    {
        return m_selection;
    }

private:
    Scene* m_context = nullptr;

    GameObject* m_selection = nullptr;
    GameObject* m_deletedGameObject = nullptr;

    void textureSelect(Shared<Texture2D>& texture);
};

}
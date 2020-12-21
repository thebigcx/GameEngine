#pragma once

#include <scene/Scene.h>
#include <core/Core.h>
#include <scene/SceneEntity.h>
#include <renderer/Framebuffer.h>

#include "FileSelectWindow.h"

class SceneHierarchy
{
public:
    SceneHierarchy();
    SceneHierarchy(const Shared<Scene>& scene);

    template<typename T, typename F>
    void drawComponent(const std::string& name, SceneEntity& entity, const F& func);

    void drawProperties(SceneEntity& entity);
    
    void setContext(const Shared<Scene>& context)
    {
        m_context = context;
    }

    void recurseTree(SceneEntity entity);

    void onImGuiRender();

    void drawSceneRenderer();

    SceneEntity& getSelectedEntity()
    {
        return m_selection;
    }

private:
    Shared<Scene> m_context;

    SceneEntity m_selection;
    SceneEntity m_deletedEntity;

    void textureSelect(Shared<Texture2D>& texture);
};
#pragma once

#include <core/Core.h>
#include <scene/Scene.h>
#include <renderer/Framebuffer.h>
#include <util/PerspectiveCamera.h>
#include <renderer/Model.h>

class MaterialsPanel
{
public:
    MaterialsPanel();
    MaterialsPanel(const Shared<Scene>& context);

    void setContext(const Shared<Scene>& context)
    {
        m_context = context;
    }

    void onImGuiRender(SceneEntity& selectedEntity);

private:
    Shared<Scene> m_context;
    Shared<Framebuffer> m_materialPreviewViewport;
    Shared<Mesh> m_sphereMesh;

    EditorCamera m_camera;

    void renderMaterialPreview(const Shared<Material>& material);
};
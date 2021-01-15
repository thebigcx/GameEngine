#pragma once

#include <core/Core.h>
#include <scene/Scene.h>
#include <renderer/Framebuffer.h>
#include <util/PerspectiveCamera.h>
#include <renderer/Model.h>

namespace Engine
{

class MaterialsPanel
{
public:
    MaterialsPanel();
    MaterialsPanel(Scene* context);

    void setContext(Scene* context) { m_context = context; }

    void onImGuiRender();

private:
    void init();

    void renderMaterialPreview(const Shared<Material>& material);

    void textureSelect(Shared<Texture2D>& texture);
    void shaderSelect(Shared<Shader>& shader);

private:
    Scene* m_context = nullptr;
    Shared<Framebuffer> m_materialPreviewViewport;
    Shared<Mesh> m_sphereMesh;
    
    DirectionalLight m_directionalLight;
    SkyLight m_skyLight;

    EditorCamera m_camera;

    std::unordered_map<int, std::string> m_texturePathInputs;
};

}
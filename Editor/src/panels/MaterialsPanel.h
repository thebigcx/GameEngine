#pragma once

#include <core/Core.h>
#include <renderer/RenderTarget.h>
#include <scene/EditorCamera.h>
#include <renderer/Model.h>
#include <renderer/Lighting.h>

namespace Engine
{

class Scene;

class MaterialsPanel
{
public:
    MaterialsPanel();
    MaterialsPanel(Scene* context);

    void setContext(Scene* context) { m_context = context; }

    void onImGuiRender();

private:
    void init();

    void renderMaterialPreview(const Reference<Material>& material);

    void textureSelect(Reference<Texture2D>& texture);
    void shaderSelect(Reference<Shader>& shader);

private:
    Scene* m_context = nullptr;
    Reference<RenderTarget> m_materialPreviewViewport;
    Reference<Mesh> m_sphereMesh;
    
    DirectionalLight m_directionalLight;
    SkyLight m_skyLight;

    EditorCamera m_camera;

    std::unordered_map<int, std::string> m_texturePathInputs;
};

}
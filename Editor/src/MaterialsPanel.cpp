#include "MaterialsPanel.h"

#include <imgui/imgui.h>

#include <renderer/RenderCommand.h>
#include <renderer/MeshFactory.h>
#include <renderer/Renderer3D.h>
#include <scene/Components.h>

MaterialsPanel::MaterialsPanel()
{
    m_materialPreviewViewport = Framebuffer::create(100, 100);

    m_camera = EditorCamera(30.f, 1280.f / 720.f, 0.1f, 100000.f);
    m_sphereMesh = MeshFactory::sphereMesh(1.f, 36, 18);
}

MaterialsPanel::MaterialsPanel(const Shared<Scene>& context)
    : m_context(context)
{
    m_materialPreviewViewport = Framebuffer::create(100, 100);

    m_camera = EditorCamera(30.f, 1280.f / 720.f, 0.1f, 100000.f);
    m_sphereMesh = MeshFactory::sphereMesh(1.f, 36, 18);
}

void textureSelect(Shared<Texture2D>& texture)
{
    char buf[128];
    strcpy(buf, texture->getPath().c_str());
    ImGui::InputText("Texture", buf, 128);

    if (ImGui::Button("Load"))
    {
        texture = Texture2D::create(std::string(buf));
    }
}

void MaterialsPanel::onImGuiRender(SceneEntity& selectedEntity)
{
    m_camera.updateView();

    ImGui::Begin("Materials");

    if (!selectedEntity)
    {
        ImGui::End();
        return;
    }

    for (auto& material : selectedEntity.getComponent<MeshComponent>().materials)
    {
        char buf[128];
        strcpy(buf, "material");
        ImGui::InputText("Name", buf, 128);

        if (ImGui::CollapsingHeader("Albedo"))
        {
            ImGui::Image(reinterpret_cast<void*>(material->albedoMap->getId()), ImVec2{50, 50});
            ImGui::SameLine();

            textureSelect(material->albedoMap);

            ImGui::SameLine();
            ImGui::Checkbox("Use", &material->usingAlbedoMap);
            ImGui::SameLine();
            ImGui::ColorEdit4("Color", &material->albedoColor.x);
        }

        if (ImGui::CollapsingHeader("Normals"))
        {
            ImGui::Image(reinterpret_cast<void*>(material->normalMap->getId()), ImVec2{50, 50});
            ImGui::SameLine();

            textureSelect(material->normalMap);

            ImGui::Checkbox("Use", &material->usingNormalMap);
        }

        if (ImGui::CollapsingHeader("Metalness"))
        {
            ImGui::Image(reinterpret_cast<void*>(material->metalnessMap->getId()), ImVec2{50, 50});
            ImGui::SameLine();

            textureSelect(material->metalnessMap);

            ImGui::SameLine();
            ImGui::Checkbox("Use", &material->usingMetalnessMap);
            ImGui::SameLine();
            ImGui::SliderFloat("Value", &material->metalness, 0.f, 100.f);
        }

        if (ImGui::CollapsingHeader("Roughness"))
        {
            ImGui::Image(reinterpret_cast<void*>(material->roughnessMap->getId()), ImVec2{50, 50});
            ImGui::SameLine();

            textureSelect(material->roughnessMap);

            ImGui::SameLine();
            ImGui::Checkbox("Use", &material->usingRoughnessMap);
            ImGui::SameLine();
            ImGui::SliderFloat("Valu", &material->roughness, 0.f, 100.f);
        }

        m_sphereMesh->materials.push_back(material);
        renderMaterialPreview(material);
    }

    ImGui::End();
}

void MaterialsPanel::renderMaterialPreview(const Shared<Material>& material)
{
    ImVec2 size = ImGui::GetContentRegionAvail();
    m_materialPreviewViewport->resize(size.x, size.y);
    m_camera.setViewportSize(size.x, size.y);

    m_materialPreviewViewport->bind();

    RenderCommand::setClearColor(math::vec4(0.f, 0.f, 0.f, 1.f));
    RenderCommand::clear(RenderCommand::defaultClearBits());

    Renderer3D::beginScene(m_camera);
    Renderer3D::submit(m_sphereMesh, math::mat4(1.f));
    Renderer3D::endScene();

    m_materialPreviewViewport->unbind();

    ImGui::Image(reinterpret_cast<void*>(m_materialPreviewViewport->getColorAttachment()), size);
}
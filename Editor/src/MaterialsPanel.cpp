#include "MaterialsPanel.h"

#include <imgui/imgui.h>

#include <renderer/RenderCommand.h>
#include <renderer/MeshFactory.h>
#include <renderer/Renderer3D.h>
#include <scene/Components.h>
#include <maths/quaternion/qua.h>
#include <maths/quaternion/qua_func.h>
#include <renderer/Assets.h>

#include "FileSelectWindow.h"

MaterialsPanel::MaterialsPanel()
{
    init();
}

MaterialsPanel::MaterialsPanel(const Shared<Scene>& context)
    : m_context(context)
{
    init();
}

void MaterialsPanel::init()
{
    m_materialPreviewViewport = Framebuffer::create(100, 100);

    m_camera = EditorCamera(30.f, 1280.f / 720.f, 0.1f, 100000.f);
    m_sphereMesh = MeshFactory::sphereMesh(2.f, 36, 18);

    m_lightSetup.setSkylight(0.1);
    m_lightSetup.setDirectionalLight({ math::vec3(0, -1, 0), math::vec3(1, 1, 1), 0.5 });
    std::vector<PointLight> lights = {
        { math::vec3(1, 1.3, 1.3), math::vec3(1, 1, 1), 1.f }
    };
    m_lightSetup.setPointLights(lights);
}

void MaterialsPanel::textureSelect(Shared<Texture2D>& texture)
{
    if (ImGui::ImageButton(reinterpret_cast<void*>(texture->getId()), ImVec2{ 50, 50 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 }))
    {
        FileSelectWindow::open(&texture);
    }

    if (FileSelectWindow::selectFile(&texture, "Choose texture...", ".png", ".jpg", ".jpeg", ".tga", ".bmp", ".pic"))
    {
        if (!FileSelectWindow::display())
        {
            if (FileSelectWindow::madeSelection())
            {
                std::cout << FileSelectWindow::getSelection() << "\n";
                texture = Texture2D::create(FileSelectWindow::getSelection());
            }
        }
    }
}

void MaterialsPanel::onImGuiRender()
{
    m_camera.updateView();

    ImGui::Begin("Materials");

    /*if (!selectedEntity || !selectedEntity.hasComponent<MeshRendererComponent>())
    {
        ImGui::End();
        return;
    }

    auto& materials = selectedEntity.getComponent<MeshRendererComponent>().materials;

    if (materials.size() == 0)
    {
        if (ImGui::Button("Create Material"))
        {
            materials.push_back(Material::create());
        }

    }*/

    //for (auto& material : materials)
    for (auto& asset : Assets::getList<Material>()->getInternalList())
    {
        if (ImGui::TreeNodeEx(asset.first.c_str()))
        {
            auto& material = asset.second;
            char buf[128];
            strcpy(buf, asset.first.c_str());
            ImGui::InputText("Name", buf, 128);

            const char* shaders[] = { "Standard" };
            const char* currentShader = shaders[0];
            if (ImGui::BeginCombo("Shader", currentShader))
            {
                for (unsigned int i = 0; i < 2; i++)
                {
                    bool isSelected = currentShader == shaders[i];
                    if (ImGui::Selectable(shaders[i], isSelected))
                    {
                        currentShader = shaders[i];
                        if (std::string(currentShader) == std::string("Standard"))
                        {
                            material->shader = Assets::get<Shader>("pbr");
                        }
                    }

                    if (isSelected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndCombo();
            }

            if (ImGui::CollapsingHeader("Albedo"))
            {
                ImGui::PushID("Albedo");

                textureSelect(material->albedoMap);

                ImGui::SameLine();
                ImGui::Checkbox("Use", &material->usingAlbedoMap);
                ImGui::SameLine();
                ImGui::ColorEdit4("Color", &material->albedoColor.x);

                ImGui::PopID();
            }

            if (ImGui::CollapsingHeader("Normals"))
            {
                ImGui::PushID("Normals");

                textureSelect(material->normalMap);

                ImGui::SameLine();
                ImGui::Checkbox("Use", &material->usingNormalMap);

                ImGui::PopID();
            }

            if (ImGui::CollapsingHeader("Metalness"))
            {
                ImGui::PushID("Metalness");

                textureSelect(material->metalnessMap);

                ImGui::SameLine();
                ImGui::Checkbox("Use", &material->usingMetalnessMap);

                ImGui::PopID();
            }

            if (ImGui::CollapsingHeader("Roughness"))
            {
                ImGui::PushID("Roughness");

                textureSelect(material->roughnessMap);

                ImGui::SameLine();
                ImGui::Checkbox("Use", &material->usingRoughnessMap);

                ImGui::PopID();
            }

            if (ImGui::CollapsingHeader("Ambient Occlusion"))
            {
                ImGui::PushID("AmbientOcclusion");

                textureSelect(material->ambientOcclusionMap);

                ImGui::SameLine();
                ImGui::Checkbox("Use", &material->usingAmbientOcclusionMap);

                ImGui::PopID();
            }

            if (ImGui::CollapsingHeader("Depth Map"))
            {
                ImGui::PushID("DepthMap");

                textureSelect(material->depthMap);

                ImGui::SameLine();
                ImGui::Checkbox("Use", &material->usingDepthMap);

                ImGui::PopID();
            }
            
            if (ImGui::CollapsingHeader("Preview"))
            {
                if (m_sphereMesh->materials.size() == 0)
                {
                    m_sphereMesh->materials.push_back(material);
                }
                else
                {
                    m_sphereMesh->materials.at(0) = material;
                }

                renderMaterialPreview(material);
            }

            ImGui::TreePop();

        }
    }

    ImGui::End();
}

void MaterialsPanel::renderMaterialPreview(const Shared<Material>& material)
{
    auto availWidth = ImGui::GetContentRegionAvailWidth();
    ImVec2 size = { availWidth, availWidth / 2 };

    m_materialPreviewViewport->resize(size.x, size.y);
    m_camera.setViewportSize(size.x, size.y);

    m_materialPreviewViewport->bind();

    RenderCommand::setClearColor(math::vec4(0.2f, 0.2f, 0.2f, 1.f));
    RenderCommand::clear(RenderCommand::defaultClearBits());

    Renderer3D::setLights(m_lightSetup);

    Renderer3D::beginScene(m_camera);
    Renderer3D::submit(m_sphereMesh, math::to_mat4(math::quat(math::vec3(math::radians(-80), 0, 0))));
    Renderer3D::endScene();

    m_materialPreviewViewport->unbind();

    ImGui::Image(reinterpret_cast<void*>(m_materialPreviewViewport->getColorAttachment()), size, ImVec2{0, 1}, ImVec2{1, 0});
}
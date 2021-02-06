#include "MaterialsPanel.h"

#include <imgui/imgui.h>

#include <renderer/RenderCommand.h>
#include <renderer/MeshFactory.h>
#include <renderer/Renderer3D.h>
#include <scene/Components.h>
#include <maths/quaternion/qua.h>
#include <maths/quaternion/qua_func.h>
#include <renderer/Assets.h>
#include <util/uuid.h>

#include "../FileDialog.h"

namespace Engine
{

MaterialsPanel::MaterialsPanel()
{
    init();
}

MaterialsPanel::MaterialsPanel(Scene* context)
    : m_context(context)
{
    init();
}

void MaterialsPanel::init()
{
    m_materialPreviewViewport = RenderTarget::create(100, 100);

    m_camera = EditorCamera(30.f, 1280.f / 720.f, 0.1f, 100000.f);
    m_sphereMesh = MeshFactory::sphereMesh(2.f, 36, 18);

    m_directionalLight = DirectionalLight(math::vec3(1, 1, 1), 0.5, math::vec3(0, 1, 0));
    m_skyLight = SkyLight(math::vec3(1, 1, 1), 0.1);
}

void MaterialsPanel::textureSelect(Reference<Texture2D>& texture)
{
    uint32_t id = texture ? texture->getId() : Texture2D::createWhiteTexture()->getId();

    // Constrain image preview to line height
    ImVec2 size = {25, 25};
    ImGui::Image(reinterpret_cast<void*>(id), ImVec2{size.y, size.y}, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    ImGui::SameLine();
        
    if (ImGui::BeginCombo("##Texture", nullptr, ImGuiComboFlags_NoPreview))
    {
        for (auto& textureAsset : Assets::getCache<Texture2D>())
        {
            ImGui::PushID(textureAsset.second.get());

            bool isSelected = texture ? texture->getId() == textureAsset.second->getId() : false;
            if (ImGui::Selectable(textureAsset.second->name.c_str(), isSelected))
            {
                texture = textureAsset.second;
            }

            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }

            ImGui::PopID();
        }

        if (ImGui::Selectable("<empty_texture>", texture == nullptr))
        {
            texture = nullptr;
        }

        ImGui::EndCombo();
    }
}

void MaterialsPanel::onImGuiRender()
{
    m_camera.updateView();

    ImGui::Begin("Materials");

    if (ImGui::CollapsingHeader("Materials"))
    {
        for (auto& asset : Assets::getCache<Material>())
        {
            ImGui::PushID(asset.first.c_str());

            if (ImGui::TreeNode(asset.second->name.c_str()))
            {
                auto& material = asset.second;

                char buf[128];
                strcpy(buf, material->name.c_str());

                ImGui::Text("Name");
                ImGui::SameLine();

                if (ImGui::InputText("##Name", buf, 128, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    material->name = std::string(buf);
                }

                ImGui::Text("Shader");
                ImGui::SameLine();

                std::string currentShader = material->shader ? material->shader->name : "<empty_shader>";

                if (ImGui::BeginCombo("##Shader", currentShader.c_str()))
                {
                    for (auto& shader : Assets::getCache<Shader>())
                    {
                        ImGui::PushID(shader.second.get());

                        bool isSelected = material->shader ? material->shader->getId() == shader.second->getId() : false;
                        if (ImGui::Selectable(shader.second->name.c_str(), isSelected))
                        {
                            material->shader = shader.second;
                        }

                        if (isSelected)
                        {
                            ImGui::SetItemDefaultFocus();
                        }

                        ImGui::PopID();
                    }

                    ImGui::EndCombo();
                }

                ImGui::NewLine();

                ImGui::Columns(2, nullptr, false);
                {
                    ImGui::PushID("Albedo");

                    textureSelect(material->albedoMap);
                    ImGui::SameLine();
                    ImGui::Text("Albedo");

                    ImGui::NextColumn();

                    ImGuiColorEditFlags flags = ImGuiColorEditFlags_NoInputs;
                    ImGui::ColorEdit3("##Color", &material->albedoColor.x, flags);
                    ImGui::NextColumn();

                    ImGui::PopID();
                }
                {
                    ImGui::PushID("Normals");

                    textureSelect(material->normalMap);
                    ImGui::SameLine();
                    ImGui::Text("Normal Map");

                    ImGui::NextColumn();
                    ImGui::NextColumn();

                    ImGui::PopID();
                }
                {
                    ImGui::PushID("Metallic");

                    textureSelect(material->metallicMap);
                    ImGui::SameLine();
                    ImGui::Text("Metallic");
                    ImGui::NextColumn();

                    ImGui::SliderFloat("##metallicScalar", &material->metallicScalar, 0.f, 1.f);
                    ImGui::NextColumn();

                    ImGui::PopID();
                }
                {
                    ImGui::PushID("Roughness");

                    textureSelect(material->roughnessMap);
                    ImGui::SameLine();
                    ImGui::Text("Roughness");
                    ImGui::NextColumn();

                    ImGui::SliderFloat("##roughnessScalar", &material->roughnessScalar, 0.f, 1.f);
                    ImGui::NextColumn();

                    ImGui::PopID();
                }
                {
                    ImGui::PushID("AmbientOcclusion");

                    textureSelect(material->ambientOcclusionMap);
                    ImGui::SameLine();
                    ImGui::Text("Ambient Occlusion");
                    ImGui::NextColumn();
                    ImGui::NextColumn();
       
                    ImGui::PopID();
                }
                {
                    ImGui::PushID("DepthMap");

                    textureSelect(material->depthMap);
                    ImGui::SameLine();
                    ImGui::Text("Height Map");
                    ImGui::NextColumn();
                    ImGui::NextColumn();

                    ImGui::PopID();
                }
                {
                    ImGui::PushID("EmissionMap");

                    textureSelect(material->emissionMap);
                    ImGui::SameLine();
                    ImGui::Text("Emission");
                    ImGui::NextColumn();
                    ImGui::NextColumn();

                    ImGui::PopID();
                }

                ImGui::Columns(1);
                
                if (ImGui::CollapsingHeader("Preview"))
                {
                    renderMaterialPreview(material);
                }

                ImGui::TreePop();
            }

            ImGui::PopID();
        }

        if (ImGui::Button("Create Material"))
        {
            auto material = Material::create();
            material->name = "New Material";
            material->uuid = Utils::genUUID();

            Assets::add<Material>(material->name, material);
        }
    }

    if (ImGui::CollapsingHeader("Shaders"))
    {
        for (auto& shader : Assets::getCache<Shader>().getInternalList())
        {
            bool opened = ImGui::TreeNodeEx(shader.second->name.c_str());

            if (opened)
            {
                ImGui::PushID(&shader.second);

                char buf[128];
                strcpy(buf, shader.second->name.c_str());

                ImGui::Text("Name");
                ImGui::SameLine();

                if (ImGui::InputText("##Name", buf, 128, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    shader.second->name = std::string(buf);
                }

                shaderSelect(shader.second);

                ImGui::PopID();

                ImGui::TreePop();
            }
        }
        
        if (ImGui::Button("Create Shader"))
        {
            FileDialog::open("createNewShader");
        }

        if (FileDialog::selectFile("createNewShader", "Choose shader...", ".glsl")) // TODO: select frag and vert shaders as multi-select in FileDialog
        {
            if (!FileDialog::display())
            {
                if (FileDialog::madeSelection())
                {
                    std::string name = FileDialog::getSelection();
                    auto shader = Shader::createFromFile(FileDialog::getSelection());
                    shader->name = name;
                    shader->uuid = Utils::genUUID();
                    Assets::add<Shader>(shader->name, shader);
                }
            }
        }
    }

    std::string deletedTexture = "";
    if (ImGui::CollapsingHeader("Textures"))
    {
        std::string oldName = "";
        std::string newName = "";
        for (auto& texture : Assets::getCache<Texture2D>().getInternalList())
        {
            bool opened = ImGui::TreeNodeEx(texture.second->name.c_str());

            if (ImGui::BeginPopupContextItem())
            {
                if (ImGui::MenuItem("Delete Texture"))
                {
                    deletedTexture = texture.first;
                }

                ImGui::EndPopup();
            }

            if (opened)
            {
                char buf[128];
                strcpy(buf, texture.second->name.c_str());

                ImGui::Text("Name");
                ImGui::SameLine();

                if (ImGui::InputText("##Name", buf, 128, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    oldName = texture.first;
                    newName = buf;
                }

                ImGui::Image(reinterpret_cast<void*>(texture.second->getId()), ImVec2{30, 30}, ImVec2{0, 1}, ImVec2{1, 0});
                ImGui::TreePop();
            }
        }

        if (oldName != "")
        {
            auto pair = Assets::getCache<Texture2D>().getInternalList().extract(oldName);
            pair.mapped()->name = newName;
            pair.key() = newName;
            Assets::getCache<Texture2D>().getInternalList().insert(std::move(pair));
        }

        if (ImGui::Button("Create Texture"))
        {
            FileDialog::open("createTexture");   
        }

        if (FileDialog::selectFile("createTexture", "Choose texture...", ".png", ".jpg", ".jpeg", ".tga", ".bmp", ".pic"))
        {
            if (!FileDialog::display())
            {
                if (FileDialog::madeSelection())
                {
                    Reference<Texture2D> texture = Texture2D::create(FileDialog::getSelection());
                    texture->name = "New Texture";
                    texture->uuid = Utils::genUUID();
                    Assets::add<Texture2D>(texture->name, texture);
                }
            }
        }
    }

    if (deletedTexture != "")
    {
        Assets::remove<Texture2D>(deletedTexture);
    }

    ImGui::End();
}

void MaterialsPanel::shaderSelect(Reference<Shader>& shader)
{
    if (ImGui::Button("Load"))
    {
        FileDialog::open(&shader);
    }

    if (FileDialog::selectFile(&shader, "Choose shader...", ".glsl")) // TODO: select frag and vert shaders as multi-select in FileDialog
    {
        if (!FileDialog::display())
        {
            if (FileDialog::madeSelection())
            {
                shader = Shader::createFromFile(FileDialog::getSelection());
            }
        }
    }
}

void MaterialsPanel::renderMaterialPreview(const Reference<Material>& material)
{
    m_sphereMesh->material = material;

    auto availWidth = ImGui::GetContentRegionAvailWidth();
    ImVec2 size = { availWidth, availWidth / 2 };

    m_materialPreviewViewport->resize(size.x, size.y);
    m_camera.setViewportSize(size.x, size.y);

    m_materialPreviewViewport->bind();

    RenderCommand::setClearColor(math::vec4(0.2f, 0.2f, 0.2f, 1.f));
    RenderCommand::clear(RenderCommand::defaultClearBits());

    Renderer3D::clearLights();
    Renderer3D::addLight(&m_directionalLight);
    Renderer3D::addLight(&m_skyLight);

    Renderer3D::beginScene(m_camera);
    Renderer3D::submit(m_sphereMesh, math::to_mat4(math::quat(math::vec3(math::radians(-80), 0, 0))));
    Renderer3D::endScene();

    m_materialPreviewViewport->unbind();

    ImGui::Image(reinterpret_cast<void*>(m_materialPreviewViewport->getColorAttachment()), size, ImVec2{0, 1}, ImVec2{1, 0});
}

}
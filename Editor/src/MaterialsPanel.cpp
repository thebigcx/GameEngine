#include "MaterialsPanel.h"

#include <imgui/imgui.h>

#include <renderer/RenderCommand.h>
#include <renderer/MeshFactory.h>
#include <renderer/Renderer3D.h>
#include <scene/Components.h>
#include <maths/quaternion/qua.h>
#include <maths/quaternion/qua_func.h>
#include <renderer/Assets.h>

#include "FileDialog.h"

namespace Engine
{

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

    m_directionalLight = DirectionalLight(math::vec3(1, 1, 1), 0.5, math::vec3(0, 1, 0));
    m_skyLight = SkyLight(math::vec3(1, 1, 1), 0.1);
}

void MaterialsPanel::textureSelect(Shared<Texture2D>& texture)
{
    bool open = false;
    if (texture)
        ImGui::ImageButton(reinterpret_cast<void*>(texture->getId()), ImVec2{ 50, 50 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    else
        ImGui::ImageButton(0, ImVec2{ 50, 50 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    
    if (open)
    {
        FileDialog::open(&texture);
    }

    if (FileDialog::selectFile(&texture, "Choose texture...", ".png", ".jpg", ".jpeg", ".tga", ".bmp", ".pic"))
    {
        if (!FileDialog::display())
        {
            if (FileDialog::madeSelection())
            {
                std::string key = FileDialog::getSelection();
                if (Assets::exists<Texture2D>(key))
                {
                    texture = Assets::get<Texture2D>(key);
                }
                else
                {
                    texture = Texture2D::create(key);
                }
            }
        }
    }
}

void MaterialsPanel::onImGuiRender()
{
    m_camera.updateView();

    ImGui::Begin("Materials");

    if (ImGui::CollapsingHeader("Materials"))
    {
        for (auto& asset : Assets::getList<Material>())
        {
            if (ImGui::TreeNodeEx(asset.first.c_str()))
            {
                auto& material = asset.second;
                char buf[128];
                strcpy(buf, asset.first.c_str());
                ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;
                flags |= ImGuiInputTextFlags_ReadOnly;// TODO: TEMP
                if (ImGui::InputText("Name", buf, 128, flags))
                {
                    if (!Assets::exists<Material>(std::string(buf)))
                    {
                        auto nh = Assets::getList<Material>().getInternalList().extract(asset.first);
                        nh.key() = std::string(buf);
                        Assets::getList<Material>().getInternalList().insert(std::move(nh));
                    }
                }

                std::string currentShader = "";

                for (auto& shader : Assets::getList<Shader>())
                {
                    if (shader.second == material->shader)
                    {
                        currentShader = shader.first;
                    }
                }

                if (ImGui::BeginCombo("Shader", currentShader.c_str()))
                {
                    for (auto& shader : Assets::getList<Shader>())
                    {
                        bool isSelected = currentShader == shader.first;
                        if (ImGui::Selectable(shader.first.c_str(), isSelected))
                        {
                            material->shader = shader.second;
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
                    ImGui::ColorEdit3("Color", &material->albedoColor.x);

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

                if (ImGui::CollapsingHeader("Metallic"))
                {
                    ImGui::PushID("Metallic");

                    textureSelect(material->metallicMap);

                    ImGui::SameLine();
                    ImGui::Checkbox("Use", &material->usingMetallicMap);

                    ImGui::SameLine();
                    ImGui::SliderFloat("##metallicScalar", &material->metallicScalar, 0.f, 32.f);

                    ImGui::PopID();
                }

                if (ImGui::CollapsingHeader("Roughness"))
                {
                    ImGui::PushID("Roughness");

                    textureSelect(material->roughnessMap);

                    ImGui::SameLine();
                    ImGui::Checkbox("Use", &material->usingRoughnessMap);

                    ImGui::SameLine();
                    ImGui::SliderFloat("##roughnessScalar", &material->roughnessScalar, 0.f, 32.f);

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
                    renderMaterialPreview(material);
                }

                ImGui::TreePop();

            }
        }
    }

    if (ImGui::CollapsingHeader("Shaders"))
    {
        for (auto& shader : Assets::getList<Shader>().getInternalList())
        {
            bool opened = ImGui::TreeNodeEx(shader.first.c_str());

            if (ImGui::BeginPopupContextItem())
            {
                if (ImGui::MenuItem("Change Name"))
                {
                    ImGui::OpenPopup("Change Shader Name");
                }

                ImGui::EndPopup();
            }

            if (opened)
            {
                ImGui::PushID(&shader.second);

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
                    Assets::add<Shader>(name, Shader::createFromFile(FileDialog::getSelection()));
                }
            }
        }
    }

    std::string deletedTexture = "";
    if (ImGui::CollapsingHeader("Textures"))
    {
        for (auto& texture : Assets::getList<Texture2D>().getInternalList())
        {
            bool opened = ImGui::TreeNodeEx(texture.first.c_str());

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
                ImGui::Image(reinterpret_cast<void*>(texture.second->getId()), ImVec2{30, 30}, ImVec2{0, 1}, ImVec2{1, 0});
                ImGui::TreePop();
            }
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
                    Shared<Texture2D> texture = Texture2D::create(FileDialog::getSelection());
                    Assets::add<Texture2D>(FileDialog::getSelection(), texture);
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

void MaterialsPanel::shaderSelect(Shared<Shader>& shader)
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
                // TODO: patch Assets key
                shader = Shader::createFromFile(FileDialog::getSelection());
            }
        }
    }
}

void MaterialsPanel::renderMaterialPreview(const Shared<Material>& material)
{
    m_sphereMesh->material = material;

    auto availWidth = ImGui::GetContentRegionAvailWidth();
    ImVec2 size = { availWidth, availWidth / 2 };

    m_materialPreviewViewport->resize(size.x, size.y);
    m_camera.setViewportSize(size.x, size.y);

    m_materialPreviewViewport->bind();

    RenderCommand::setClearColor(math::vec4(0.2f, 0.2f, 0.2f, 1.f));
    RenderCommand::clear(RenderCommand::defaultClearBits());

    //m_directionalLight.addToRenderer();
    //m_skyLight.addToRenderer();

    Renderer3D::beginScene(m_camera);
    Renderer3D::submit(m_sphereMesh, math::to_mat4(math::quat(math::vec3(math::radians(-80), 0, 0))));
    Renderer3D::endScene();

    m_materialPreviewViewport->unbind();

    ImGui::Image(reinterpret_cast<void*>(m_materialPreviewViewport->getColorAttachment()), size, ImVec2{0, 1}, ImVec2{1, 0});
}

}
#include "SceneHierarchy.h"

#include <imgui/imgui.h>

#include <scene/Components.h>

#include <renderer/Model.h>
#include <renderer/RenderCommand.h>
#include <renderer/Renderer3D.h>
#include <util/PerspectiveCamera.h>
#include <renderer/MeshFactory.h>
#include <renderer/Renderer.h>
#include <renderer/Assets.h>
#include <util/io/Files.h>

namespace Engine
{

SceneHierarchy::SceneHierarchy()
{
    
}

SceneHierarchy::SceneHierarchy(const Shared<Scene>& scene)
    : m_context(scene)
{
    
}

void SceneHierarchy::recurseTree(GameObject& object)
{
    ImGui::PushID(&object);

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;
    const char* tag;
    if (!object.hasComponent<TagComponent>())
    {
        //ImGui::PopID();
        object.addComponent<TagComponent>("?");
        //return;
    }
    else
        tag = object.getComponent<TagComponent>().tag.c_str();
    
    bool objectExpanded = ImGui::TreeNodeEx(tag, flags);

    if (ImGui::IsItemClicked())
    {
        m_selection = &object;
    }
    
    bool deleted = false;
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::MenuItem("Create Child"))
        {
            auto child = object.addChild();
            child->addComponent<TagComponent>("Untitled Child");
        }

        if (ImGui::MenuItem("Delete Game Object"))
        {
            deleted = true;
        }

        ImGui::EndPopup();
    }

    if (deleted)
    {
        m_selection = nullptr;
        m_deletedGameObject = &object;
    }

    if (objectExpanded)
    {
        for (auto& object : object.getChildren())
        {
            recurseTree(object);
        }
        
        
        ImGui::TreePop();
    }

    ImGui::PopID();
}

void SceneHierarchy::onImGuiRender()
{
    ImGui::Begin("Scene Hierarchy");

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_CollapsingHeader;
    bool gameObjectsOpen = ImGui::CollapsingHeader("Game Objects");

    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::MenuItem("Create Game Object"))
        {
            auto object = m_context->createGameObject("Untitled Game Object");
            m_selection = object;
        }

        if (ImGui::MenuItem("Import 3D Model"))
        {
            FileDialog::open(reinterpret_cast<const void*>("modelload"));
        }

        ImGui::EndPopup();
    }

    if (FileDialog::selectFile(reinterpret_cast<const void*>("modelload"), "Choose model...", ".obj", ".fbx", ".blend", ".3ds", ".gltf"))
    {
        if (!FileDialog::display())
        {
            if (FileDialog::madeSelection())
            {
                Shared<Model> model = Model::load(FileDialog::getSelection());

                auto object = m_context->createGameObject("Model");

                int meshID = 0;
                for (auto& mesh : model->meshes)
                {
                    auto child = object->addChild();
                    child->addComponent<TagComponent>(std::string("Mesh_") + std::to_string(meshID));

                    child->addComponent<MeshComponent>();
                    child->getComponent<MeshComponent>().mesh = mesh;
                    child->getComponent<MeshComponent>().meshID = meshID;
                    child->getComponent<MeshComponent>().filePath = FileDialog::getSelection();
                    child->addComponent<TransformComponent>();

                    Assets::addIfNotExists(Assets::generateID<Material>(), child->getComponent<MeshComponent>().mesh->material);

                    auto& meshRenderer = child->addComponent<MeshRendererComponent>();

                    meshRenderer.materials.push_back(mesh->material);

                    meshID++;
                }

                m_selection = object;
            }
        }
    }

    if (gameObjectsOpen)
    {   
        m_deletedGameObject = nullptr;
        auto& gameObjects = m_context->getRootGameObject().getChildren();
        for (auto& gameObject : gameObjects)
        {
            recurseTree(gameObject);
        }

        if (m_deletedGameObject)
        {
            m_deletedGameObject->destroy();
        }
    }

    ImGui::End();

    ImGui::Begin("Properties");

    if (m_selection != nullptr)
    {
        if (m_selection->hasComponent<TagComponent>())
            drawProperties(*m_selection);
    }

    ImGui::End();

    drawSceneRenderer();

    ImGui::Begin("Debug");

    ImGui::End();

    ImGui::Begin("Environment");

    float exposure = Renderer::hdrExposure;
    ImGui::SliderFloat("Exposure", &exposure, 0.f, 5.f);
    Renderer::hdrExposure = exposure;

    ImGui::End();
}

#define ADD_COMPONENT(type, str) if (!object.hasComponent<type>())\
                                {\
                                    if (ImGui::MenuItem(str)) {\
                                        object.addComponent<type>();\
                                        ImGui::CloseCurrentPopup();\
                                    }\
                                }

void SceneHierarchy::drawProperties(GameObject& object)
{
    char buf[128];
    strcpy(buf, object.getComponent<TagComponent>().tag.c_str());
    ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;
    ImGui::InputText("Name", buf, 128, flags);
    object.getComponent<TagComponent>().tag = std::string(buf);

    if (ImGui::Button("Add Component"))
    {
        ImGui::OpenPopup("AddComponent");
    }

    if (ImGui::BeginPopup("AddComponent"))
    {
        ADD_COMPONENT(TransformComponent, "Transform");
        ADD_COMPONENT(SpriteRendererComponent, "Sprite Renderer");
        ADD_COMPONENT(CameraComponent, "Camera");
        ADD_COMPONENT(BoxCollider2DComponent, "Box Collider 2D");
        ADD_COMPONENT(MeshComponent, "Mesh");
        ADD_COMPONENT(SkyLightComponent, "Sky Light");
        ADD_COMPONENT(DirectionalLightComponent, "Directional Light");
        ADD_COMPONENT(PointLightComponent, "Point Light");
        ADD_COMPONENT(MeshRendererComponent, "Mesh Renderer");
        ADD_COMPONENT(LuaScriptComponent, "Script");

        ImGui::EndPopup();
    }

    drawComponent<TransformComponent>("Transform", object, [](auto& component)
    {
        ImGui::PushID("Transform");

        math::vec3& translation = component.translation;
        ImGui::DragFloat3("Translation", &translation.x);

        math::vec3& rotation = component.rotation;
        ImGui::DragFloat3("Rotation", &rotation.x);

        math::vec3& scale = component.scale;
        ImGui::DragFloat3("Scale", &scale.x);

        ImGui::PopID();
    });

    drawComponent<CameraComponent>("Camera", object, [](auto& component)
    {
        ImGui::Checkbox("Primary", &component.primary);

        auto& camera = component.camera;

        const char* projectionTypes[] = { "Perspective", "Orthographic" };
        const char* currentProjectionType = projectionTypes[(int)camera.getProjectionType()];
        if (ImGui::BeginCombo("Projection", currentProjectionType))
        {
            for (unsigned int i = 0; i < 2; i++)
            {
                bool isSelected = currentProjectionType == projectionTypes[i];
                if (ImGui::Selectable(projectionTypes[i], isSelected))
                {
                    currentProjectionType = projectionTypes[i];
                    camera.setProjectionType((ProjectionType)i);
                }

                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndCombo();
        }

        if (camera.getProjectionType() == ProjectionType::Orthographic)
        {
            float size = camera.getOrthoSize();
            if (ImGui::DragFloat("Size", &size))
            {
                camera.setOrthoSize(size);
            }

            float near = camera.getOrthoNear();
            if (ImGui::DragFloat("Near", &near))
            {
                camera.setOrthoNear(near);
            }

            float far = camera.getOrthoFar();
            if (ImGui::DragFloat("Far", &far))
            {
                camera.setOrthoFar(far);
            }
        }
        else if (camera.getProjectionType() == ProjectionType::Perspective)
        {
            float fov = camera.getPerspectiveFov();
            if (ImGui::DragFloat("FOV", &fov))
            {
                camera.setPerspectiveFov(fov);
            }

            float near = camera.getPerspectiveNear();
            if (ImGui::DragFloat("Near", &near))
            {
                camera.setPerspectiveNear(near);
            }

            float far = camera.getPerspectiveFar();
            if (ImGui::DragFloat("Far", &far))
            {
                camera.setPerspectiveFar(far);
            }
        }
    });

    drawComponent<SpriteRendererComponent>("Sprite Renderer", object, [&](auto& component)
    {
        ImGui::ColorEdit4("Color", &(component.color.x));

        if (component.texture == nullptr)
        {
            component.texture = Assets::get<Texture2D>("white_texture");
        }
        
        std::string currentTexture = Assets::find<Texture2D>(component.texture);
        
        if (ImGui::BeginCombo("Texture", currentTexture.c_str()))
        {
            for (auto& texture : Assets::getList<Texture2D>())
            {
                bool isSelected = currentTexture == texture.first;
                if (ImGui::Selectable(texture.first.c_str(), isSelected))
                {
                    component.texture = texture.second;
                }

                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndCombo();
        }

        ImGui::Checkbox("Using Texture Region", &component.usingTexRect);
        ImGui::DragFloat("X", &component.textureRect.x);
        ImGui::DragFloat("Y", &component.textureRect.y);
        ImGui::DragFloat("Width", &component.textureRect.w);
        ImGui::DragFloat("Height", &component.textureRect.h);
        
    });

    drawComponent<TextRendererComponent>("Text Renderer", object, [](auto& component)
    {
        ImGui::ColorEdit4("Color", &component.color.x);
        ImGui::Text("Font selection window goes here...");

        char buf[128];
        ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;
        ImGui::InputText("String", buf, 128, flags);
        component.text = std::string(buf);
    });

    drawComponent<MeshComponent>("Mesh", object, [&object](auto& component)
    {
        char buf[128];
        if (component.filePath == "")
        {
            strcpy(buf, "<empty_mesh>");
        }
        else
        {
            strcpy(buf, component.filePath.c_str());
        }

        ImGuiInputTextFlags flags = ImGuiInputTextFlags_ReadOnly;
        ImGui::InputText("", buf, 128, flags);

        ImGui::SameLine();
        if (ImGui::Button("..."))
        {
            FileDialog::open(&component);
        }

        memset(buf, 0, 128);
        strcpy(buf, std::to_string(component.meshID).c_str());
        flags = ImGuiInputTextFlags_EnterReturnsTrue;
        if (ImGui::InputText("ID", buf, 128, flags))
        {
            int meshID = std::stoi(std::string(buf));
            component.meshID = meshID;
        }

        if (FileDialog::selectFile(&component, "Choose mesh...", ".obj", ".fbx", ".blend", ".3ds", ".gltf"))
        {
            if (!FileDialog::display())
            {
                if (FileDialog::madeSelection())
                {
                    component.filePath = FileDialog::getSelection();
                    component.mesh = Mesh::load(component.filePath, component.meshID);
                }
            }
        }
    });

    drawComponent<SkyLightComponent>("Sky Light", object, [](auto& component)
    {
        ImGui::PushID("Skylight");
        ImGui::DragFloat("Intensity", &component.intensity, 0.01f, 0.f);
        ImGui::PopID();
    });

    drawComponent<DirectionalLightComponent>("Directional Light", object, [](auto& component)
    {
        ImGui::PushID("Directionallight");
        ImGui::ColorEdit3("Radiance", &component.radiance.x);
        ImGui::DragFloat("Intensity", &component.intensity, 0.01f, 0.f);
        ImGui::PopID();
    });

    drawComponent<PointLightComponent>("Point Light", object, [](auto& component)
    {
        ImGui::PushID("Pointlight");
        ImGui::ColorEdit4("Radiance", &component.radiance.x);
        ImGui::DragFloat("Intensity", &component.intensity, 0.01f, 0.f);
        ImGui::DragFloat("Attenuation", &component.attenuation, 0.01f, 0.f);
        ImGui::PopID();
    });

    drawComponent<MeshRendererComponent>("Mesh Renderer", object, [](auto& component)
    {
        if (ImGui::TreeNode("Materials"))
        {
            char buf[128];
            strcpy(buf, std::to_string(component.materials.size()).c_str());

            ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;
            if (ImGui::InputText("Size", buf, 128, flags))
            {
                bool isInteger = std::string(buf).find_first_not_of("0123456789 ") == std::string::npos;

                if (isInteger && std::string(buf).size() > 0)
                {
                    unsigned int materialCount = math::max(std::stoi(buf), 1);

                    if (materialCount > component.materials.size())
                    {
                        component.materials.push_back(Material::create());
                    }
                    else if (materialCount < component.materials.size())
                    {
                        for (size_t i = 0; i < component.materials.size() - materialCount; i++)
                        {
                            component.materials.pop_back();
                        }
                    }
                }
            }

            ImGui::Columns(2);

            int element = 0;
            for (auto& material : component.materials)
            {
                ImGui::PushID(&material);

                ImGui::Text((std::string("Material ") + std::to_string(element)).c_str());
                ImGui::NextColumn();
                
                std::vector<const char*> materialNames;
                for (auto& asset : Assets::getList<Material>())
                {
                    materialNames.push_back(asset.first.c_str());
                }

                const char* currentMaterial = materialNames[0];
                if (ImGui::BeginCombo("##materialCombo", currentMaterial))
                {
                    for (unsigned int i = 0; i < materialNames.size(); i++)
                    {
                        bool isSelected = currentMaterial == materialNames[i];
                        if (ImGui::Selectable(materialNames[i], isSelected))
                        {
                            currentMaterial = materialNames[i];
                            material = Assets::get<Material>(currentMaterial);
                        }

                        if (isSelected)
                        {
                            ImGui::SetItemDefaultFocus();
                        }
                    }

                    ImGui::EndCombo();
                }
                
                ImGui::NextColumn();
                element++;

                ImGui::PopID();
            }
            
            ImGui::Columns(1);

            ImGui::TreePop();
        }
    });

    drawComponent<LuaScriptComponent>("Lua Script", object, [](auto& component)
    {
        char buf[128];
        strcpy(buf, component.filePath.c_str());
        ImGuiInputTextFlags flags = ImGuiInputTextFlags_ReadOnly;
        ImGui::InputText("##luaScriptPath", buf, 128, flags);
        ImGui::SameLine();

        if (ImGui::Button("...##luaScriptPathSelect"))
        {
            FileDialog::open("luaScriptPathSelect");
        }

        if (FileDialog::selectFile("luaScriptPathSelect", "Choose script...", ".lua"))
        {
            if (!FileDialog::display())
            {
                if (FileDialog::madeSelection())
                {
                    component.filePath = FileDialog::getSelection();
                    component.source = Files::readFile(component.filePath);
                }
            }
        }
    });
}

template<typename T, typename F>
void SceneHierarchy::drawComponent(const std::string& name, GameObject& object, const F& func)
{
    if (!object.hasComponent<T>())
    {
        return;
    }

    auto& component = object.getComponent<T>();

    ImGui::Separator();

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;
    flags |= ImGuiTreeNodeFlags_CollapsingHeader;
    bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, name.c_str());

    bool deleted = false;
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::MenuItem("Remove Component"))
        {
            deleted = true;
        }

        ImGui::EndPopup();
    }

    if (opened)
    {
        func(component);
    }

    if (deleted)
    {
        object.removeComponent<T>();
    }
}

void SceneHierarchy::textureSelect(Shared<Texture2D>& texture)
{
    if (ImGui::Button("..."))
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

void SceneHierarchy::drawSceneRenderer()
{
    ImGui::Begin("Scene Renderer");

    if (ImGui::CollapsingHeader("Shadows"))
    {

    }

    if (ImGui::CollapsingHeader("Bloom"))
    {

    }

    ImGui::End();
}

}
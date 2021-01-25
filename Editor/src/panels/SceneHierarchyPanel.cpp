#include "SceneHierarchyPanel.h"

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

SceneHierarchyPanel::SceneHierarchyPanel()
{
    
}

SceneHierarchyPanel::SceneHierarchyPanel(Scene* scene)
    : m_context(scene)
{
    
}

void SceneHierarchyPanel::recurseTree(GameObject& object)
{
    ImGui::PushID(&object);

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;
    const char* tag = object.getComponent<TagComponent>().tag.c_str();
    
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

void SceneHierarchyPanel::onImGuiRender()
{
    ImGui::Begin("Scene Hierarchy");

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_CollapsingHeader;
    bool gameObjectsOpen = ImGui::CollapsingHeader("Game Objects");

    if (ImGui::BeginPopupContextWindow(0, 1, false))
    {
        if (ImGui::BeginMenu("Create"))
        {
            if (ImGui::MenuItem("Empty Game Object"))
            {
                auto object = m_context->createGameObject("New Game Object");
                m_selection = object;
            }

            if (ImGui::MenuItem("Camera"))
            {
                auto object = m_context->createGameObject("New Camera");
                object->addComponent<TransformComponent>();
                object->addComponent<CameraComponent>();
                m_selection = object;
            }

            if (ImGui::MenuItem("Sprite"))
            {
                auto object = m_context->createGameObject("New Sprite");
                object->addComponent<TransformComponent>();
                object->addComponent<SpriteRendererComponent>();
                m_selection = object;
            }

            if (ImGui::MenuItem("Mesh"))
            {
                auto object = m_context->createGameObject("New Mesh");
                object->addComponent<TransformComponent>();
                object->addComponent<MeshComponent>();
                object->addComponent<MeshRendererComponent>();
                m_selection = object;
            }

            if (ImGui::MenuItem("Directional Light"))
            {
                auto object = m_context->createGameObject("Directional Light");
                object->addComponent<TransformComponent>();
                object->addComponent<DirectionalLightComponent>();
                m_selection = object;
            }

            ImGui::EndMenu();
        }

        // TODO!!!: IMPORTANT!!!!! change the 3d model system, force the use of a seperate .obj or .fbx file for each mesh
        if (ImGui::MenuItem("Import 3D Model"))
        {
            //FileDialog::open(reinterpret_cast<const void*>("modelload"));
        }

        ImGui::EndPopup();
    }
    /*
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
    */

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
        drawProperties(*m_selection);
    }

    ImGui::End();
}

#define ADD_COMPONENT(type, str) if (!object.hasComponent<type>())\
                                {\
                                    if (ImGui::Button(str)) {\
                                        object.addComponent<type>();\
                                        ImGui::CloseCurrentPopup();\
                                    }\
                                }

void SceneHierarchyPanel::drawProperties(GameObject& object)
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
        ADD_COMPONENT(CSharpScriptComponent, "Script");

        ImGui::EndPopup();
    }

    drawComponent<TransformComponent>("Transform", object, [](auto& component)
    {
        ImGui::Columns(2);

        ImGui::PushID("Transform");

        ImGui::Text("Translation");
        ImGui::NextColumn();
        math::vec3& translation = component.translation;
        ImGui::DragFloat3("##T", &translation.x);
        ImGui::NextColumn();

        ImGui::Text("Rotation");
        ImGui::NextColumn();
        math::vec3& rotation = component.rotation;
        ImGui::DragFloat3("##R", &rotation.x);
        ImGui::NextColumn();

        ImGui::Text("Scale");
        ImGui::NextColumn();
        math::vec3& scale = component.scale;
        ImGui::DragFloat3("##S", &scale.x);
        ImGui::NextColumn();

        ImGui::PopID();

        ImGui::Columns(1);
    });

    drawComponent<CameraComponent>("Camera", object, [](auto& component)
    {
        ImGui::Columns(2);

        ImGui::Text("Primary");
        ImGui::NextColumn();
        ImGui::Checkbox("##Primary", &component.primary);
        ImGui::NextColumn();

        auto& camera = component.camera;

        ImGui::Text("Projection");
        ImGui::NextColumn();

        const char* projectionTypes[] = { "Perspective", "Orthographic" };
        const char* currentProjectionType = projectionTypes[(int)camera.getProjectionType()];
        if (ImGui::BeginCombo("##Projection", currentProjectionType))
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

        ImGui::NextColumn();

        if (camera.getProjectionType() == ProjectionType::Orthographic)
        {
            float size = camera.getOrthoSize();
            ImGui::Text("Size");
            ImGui::NextColumn();
            if (ImGui::DragFloat("##Size", &size))
            {
                camera.setOrthoSize(size);
            }
            ImGui::NextColumn();

            float near = camera.getOrthoNear();
            ImGui::Text("Near");
            ImGui::NextColumn();
            if (ImGui::DragFloat("##Near", &near))
            {
                camera.setOrthoNear(near);
            }
            ImGui::NextColumn();

            float far = camera.getOrthoFar();
            ImGui::Text("Far");
            ImGui::NextColumn();
            if (ImGui::DragFloat("##Far", &far))
            {
                camera.setOrthoFar(far);
            }
            ImGui::NextColumn();
        }
        else if (camera.getProjectionType() == ProjectionType::Perspective)
        {
            float fov = math::degrees(camera.getPerspectiveFov());
            ImGui::Text("FOV");
            ImGui::NextColumn();
            if (ImGui::SliderFloat("##FOV", &fov, 0, 180));
            {
                camera.setPerspectiveFov(math::radians(fov));
            }
            ImGui::NextColumn();

            float near = camera.getPerspectiveNear();
            ImGui::Text("Near");
            ImGui::NextColumn();
            if (ImGui::DragFloat("##Near", &near))
            {
                camera.setPerspectiveNear(near);
            }
            ImGui::NextColumn();

            float far = camera.getPerspectiveFar();
            ImGui::Text("Far");
            ImGui::NextColumn();
            if (ImGui::DragFloat("##Far", &far))
            {
                camera.setPerspectiveFar(far);
            }
            ImGui::NextColumn();
        }

        ImGui::Text("HDR");
        ImGui::NextColumn();
        bool hdr = false;
        ImGui::Checkbox("##HDR", &hdr);
        ImGui::NextColumn();

        ImGui::Columns(1);
    });

    drawComponent<SpriteRendererComponent>("Sprite Renderer", object, [&](auto& component)
    {
        ImGui::Columns(2);
        
        ImGui::Text("Color");
        ImGui::NextColumn();
        if (ImGui::ColorButton("##colpicker", { component.color.r, component.color.g, component.color.b, component.color.a }))
        {
            ImGui::OpenPopup("colorPickerEnabled");
        }
        if (ImGui::BeginPopup("colorPickerEnabled"))
        {
            ImGui::ColorPicker4("##Color", &(component.color.x));
            ImGui::EndPopup();
        }

        ImGui::NextColumn();

        if (component.texture == nullptr)
        {
            component.texture = Texture2D::createWhiteTexture();
        }
        
        ImGui::Text("Texture");
        ImGui::NextColumn();

        std::string currentTexture = component.texture != nullptr ? component.texture->name : "<empty_texture>";
        
        if (ImGui::BeginCombo("##Texture", currentTexture.c_str()))
        {
            for (auto& texture : Assets::getList<Texture2D>())
            {
                ImGui::PushID(texture.second.get());

                bool isSelected = component.texture ? component.texture->getId() == texture.second->getId() : false;
                if (ImGui::Selectable(texture.second->name.c_str(), isSelected))
                {
                    component.texture = texture.second;
                }

                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }

                ImGui::PopID();
            }

            ImGui::EndCombo();
        }

        ImGui::NextColumn();

        ImGui::Text("Using Texture Region");
        ImGui::NextColumn();
        ImGui::Checkbox("##usingTextureRegion", &component.usingTexRect);
        ImGui::NextColumn();

        if (component.usingTexRect)
        {
            ImGui::Text("Position");
            ImGui::NextColumn();
            ImGui::DragFloat2("##Position", &component.textureRect.x);
            ImGui::NextColumn();

            ImGui::Text("Size");
            ImGui::NextColumn();
            ImGui::DragFloat2("##Size", &component.textureRect.w);
            ImGui::NextColumn();
        }

        ImGui::Columns(1);
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
        ImGui::Columns(2);

        char buf[128];
        if (component.filePath == "")
        {
            strcpy(buf, "<empty_mesh>");
        }
        else
        {
            strcpy(buf, component.filePath.c_str());
        }

        ImGui::Text("Filepath");
        ImGui::NextColumn();

        ImGuiInputTextFlags flags = ImGuiInputTextFlags_ReadOnly;
        ImGui::InputText("", buf, 128, flags);

        ImGui::SameLine();
        if (ImGui::Button("..."))
        {
            FileDialog::open(&component);
        }

        ImGui::NextColumn();

        ImGui::Text("ID");
        ImGui::NextColumn();

        memset(buf, 0, 128);
        strcpy(buf, std::to_string(component.meshID).c_str());
        flags = ImGuiInputTextFlags_EnterReturnsTrue;
        if (ImGui::InputText("##ID", buf, 128, flags))
        {
            int meshID = std::stoi(std::string(buf));
            component.meshID = meshID;
            component.mesh = Mesh::load(component.filePath, component.meshID);
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

        ImGui::NextColumn();

        ImGui::Columns(1);
    });

    drawComponent<SkyLightComponent>("Sky Light", object, [](auto& component)
    {
        ImGui::Columns(2);
        ImGui::PushID("SkyLight");

        ImGui::Text("Radiance");
        ImGui::NextColumn();
        ImGui::ColorEdit3("##Radiance", &component.light.radiance.r);
        ImGui::NextColumn();

        ImGui::Text("Intensity");
        ImGui::NextColumn();
        ImGui::DragFloat("##Intensity", &component.light.intensity, 0.01f, 0.f);
        ImGui::PopID();
        ImGui::Columns(1);
    });

    drawComponent<DirectionalLightComponent>("Directional Light", object, [](auto& component)
    {
        ImGui::Columns(2);
        ImGui::PushID("Directionallight");

        ImGui::Text("Radiance");
        ImGui::NextColumn();
        ImGui::ColorEdit3("##Radiance", &component.light.radiance.r);
        ImGui::NextColumn();

        ImGui::Text("Intensity");
        ImGui::NextColumn();
        ImGui::DragFloat("##Intensity", &component.light.intensity, 0.01f, 0.f);
        ImGui::PopID();
        ImGui::Columns(1);
    });

    drawComponent<PointLightComponent>("Point Light", object, [](auto& component)
    {
        ImGui::Columns(2);
        ImGui::PushID("PointLight");

        ImGui::Text("Radiance");
        ImGui::NextColumn();
        ImGui::ColorEdit3("##Radiance", &component.light.radiance.r);
        ImGui::NextColumn();

        ImGui::Text("Intensity");
        ImGui::NextColumn();
        ImGui::DragFloat("##Intensity", &component.light.intensity, 0.01f, 0.f);
        ImGui::PopID();
        ImGui::Columns(1);
    });

    drawComponent<MeshRendererComponent>("Mesh Renderer", object, [](auto& component)
    {
        ImGui::Columns(2);

        ImGui::Text("Material");
        ImGui::NextColumn();

        // Material select

        std::string name = component.material != nullptr ? component.material->name : "<empty_material>";

        if (ImGui::BeginCombo("##materialSelect", name.c_str()))
        {
            for (auto& material : Assets::getList<Material>().getInternalList())
            {
                ImGui::PushID(material.second.get());

                bool isSelected = material.second == component.material;
                if (ImGui::Selectable(material.second->name.c_str(), isSelected))
                {
                    component.material = material.second;
                    isSelected = true;
                }

                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }

                ImGui::PopID();
            }

            ImGui::EndCombo();
        }

        ImGui::NextColumn();

        ImGui::Text("Cast Shadows");
        ImGui::NextColumn();

        ImGui::Checkbox("##castShadows", &component.castShadows);
        ImGui::NextColumn();

        ImGui::Columns(1);
    });

    drawComponent<CSharpScriptComponent>("C# Script", object, [](auto& component)
    {
        ImGui::Columns(2);

        ImGui::Text("Filepath");
        ImGui::NextColumn();

        char buf[128];
        strcpy(buf, component.filepath.c_str());
        ImGuiInputTextFlags flags = ImGuiInputTextFlags_ReadOnly;
        ImGui::InputText("##csharpScriptPath", buf, 128, flags);
        ImGui::SameLine();

        if (ImGui::Button("...##csharpScriptPathSelect"))
        {
            FileDialog::open("csharpScriptPathSelect");
        }

        if (FileDialog::selectFile("csharpScriptPathSelect", "Choose script...", ".cs"))
        {
            if (!FileDialog::display())
            {
                if (FileDialog::madeSelection())
                {
                    component.filepath = FileDialog::getSelection();
                }
            }
        }

        ImGui::NextColumn();
        ImGui::Columns(1);
    });
}

template<typename T, typename F>
void SceneHierarchyPanel::drawComponent(const std::string& name, GameObject& object, const F& func)
{
    if (!object.hasComponent<T>())
    {
        return;
    }

    auto& component = object.getComponent<T>();

    ImGui::Separator();

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;
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

        ImGui::TreePop();
    }

    if (deleted)
    {
        object.removeComponent<T>();
    }
}

void SceneHierarchyPanel::textureSelect(Shared<Texture2D>& texture)
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

}
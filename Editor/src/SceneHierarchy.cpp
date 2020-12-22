#include "SceneHierarchy.h"

#include <imgui/imgui.h>

#include <scene/Components.h>
#include <scene/SceneEntity.h>
#include <renderer/Model.h>
#include <renderer/RenderCommand.h>
#include <renderer/Renderer3D.h>
#include <util/PerspectiveCamera.h>
#include <renderer/MeshFactory.h>
#include <renderer/Renderer.h>
#include <renderer/Assets.h>

SceneHierarchy::SceneHierarchy()
{
    
}

SceneHierarchy::SceneHierarchy(const Shared<Scene>& scene)
    : m_context(scene)
{
    
}

void SceneHierarchy::recurseTree(SceneEntity entity)
{
    ImGui::PushID(entity.getHandle());

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;
    const char* tag;
    if (!entity.hasComponent<TagComponent>())
        return;
    else
        tag = entity.getComponent<TagComponent>().tag.c_str();
    
    bool entityExpanded = ImGui::TreeNodeEx(tag, flags);

    if (ImGui::IsItemClicked())
    {
        m_selection = entity;
    }
    
    bool deleted = false;
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::MenuItem("Create Child"))
        {
            entity.addChild("Untitled Child");
        }

        if (ImGui::MenuItem("Delete Game Object"))
        {
            deleted = true;
        }

        ImGui::EndPopup();
    }

    if (deleted)
    {
        m_selection = SceneEntity::createNull(m_context.get());
        m_deletedEntity = entity;
    }

    if (entityExpanded)
    {
        for (auto& entity : entity.getChildren())
        {
            recurseTree(entity);
        }
        
        
        ImGui::TreePop();
    }

    ImGui::PopID();
}

void SceneHierarchy::onImGuiRender()
{
    ImGui::Begin("Scene Hierarchy");

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_CollapsingHeader;
    bool gameObjectsOpen = ImGui::TreeNodeEx("Game Objects", flags);

    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::MenuItem("Create Game Object"))
        {
            auto entity = m_context->createEntity("Untitled Game Object");
            m_selection = entity;
        }

        if (ImGui::MenuItem("Import 3D Model"))
        {
            FileSelectWindow::open(reinterpret_cast<const void*>("modelload"));
        }

        ImGui::EndPopup();
    }

    if (FileSelectWindow::selectFile(reinterpret_cast<const void*>("modelload"), "Choose model...", ".obj", ".fbx", ".blend", ".3ds"))
    {
        if (!FileSelectWindow::display())
        {
            if (FileSelectWindow::madeSelection())
            {
                Shared<Model> model = Model::loadModel(FileSelectWindow::getSelection());

                auto entity = m_context->createEntity("Model");

                int meshID = 0;
                for (auto& mesh : model->meshes)
                {
                    auto childID = entity.addChild("Mesh");
                    SceneEntity child(childID, m_context.get());

                    child.addComponent<MeshComponent>();
                    child.getComponent<MeshComponent>().mesh = mesh;
                    child.getComponent<MeshComponent>().meshID = meshID;
                    child.getComponent<MeshComponent>().filePath = FileSelectWindow::getSelection();
                    child.addComponent<TransformComponent>();

                    auto& meshRenderer = child.addComponent<MeshRendererComponent>();

                    int i = 0;
                    int index = Assets::getAssetCount<Material>() - 1;
                    for (auto& material : mesh->materials)
                    {
                        bool needToLoad = true;
                        for (auto& materialLoaded : Assets::getList<Material>()->getInternalList())
                        {
                            if (*material == *materialLoaded.second)
                            {
                                needToLoad = false;
                                meshRenderer.materials.push_back(materialLoaded.second);
                                break; // TODO: fix so this is actually works
                            }
                        }

                        if (needToLoad)
                        {
                            Assets::add<Material>(std::string("material_") + std::to_string(index), material);
                            meshRenderer.materials.push_back(material);
                            // TODO: use another way of creating ids
                            index++;
                        }

                        i++;
                    }

                    meshID++;
                }

                m_selection = entity;
            }
        }
    }

    if (gameObjectsOpen)
    {   
        m_deletedEntity = SceneEntity::createNull(m_context.get());
        m_context->getRegistry().each([&](Entity* entityHandle)
        {
            recurseTree(SceneEntity(entityHandle, m_context.get()));
        });

        if (m_deletedEntity)
        {
            m_context->destroyEntity(m_deletedEntity);
        }
    }

    ImGui::End();

    ImGui::Begin("Properties");

    if (m_selection.getHandle() != nullptr)
    {
        if (m_selection.hasComponent<TagComponent>())
            drawProperties(m_selection);
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

#define ADD_COMPONENT(type, str) if (!entity.hasComponent<type>())\
                                {\
                                    if (ImGui::MenuItem(str)) {\
                                        entity.addComponent<type>();\
                                        ImGui::CloseCurrentPopup();\
                                    }\
                                }

void SceneHierarchy::drawProperties(SceneEntity& entity)
{
    char buf[128];
    strcpy(buf, entity.getComponent<TagComponent>().tag.c_str());
    ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;
    ImGui::InputText("Name", buf, 128, flags);
    entity.getComponent<TagComponent>().tag = std::string(buf);

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

        ImGui::EndPopup();
    }

    drawComponent<TransformComponent>("Transform", entity, [](auto& component)
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

    drawComponent<CameraComponent>("Camera", entity, [](auto& component)
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

    drawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [&](auto& component)
    {
        ImGui::ColorEdit4("Color", &(component.color.x));

        if (component.texture == nullptr)
        {
            component.texture = Texture2D::create(1, 1);
            uint32_t white = 0xffffffff;
            component.texture->setData(0, 0, 1, 1, &white);
        }
        
        char buf[128];
        if (component.texture->getPath() == "")
        {
            strcpy(buf, "<empty_texture>");
        }
        else
        {
            strcpy(buf, component.texture->getPath().c_str());
        }

        ImGuiInputTextFlags flags = ImGuiInputTextFlags_ReadOnly;
        ImGui::InputText("", buf, 128, flags);
        
        ImGui::SameLine();
        textureSelect(component.texture);

        ImGui::Checkbox("Using Texture Region", &component.usingTexRect);
        ImGui::DragFloat("X", &component.textureRect.x);
        ImGui::DragFloat("Y", &component.textureRect.y);
        ImGui::DragFloat("Width", &component.textureRect.w);
        ImGui::DragFloat("Height", &component.textureRect.h);
        
    });

    drawComponent<TextRendererComponent>("Text Renderer", entity, [](auto& component)
    {
        ImGui::ColorEdit4("Color", &component.color.x);
        ImGui::Text("Font selection window goes here...");

        char buf[128];
        ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;
        ImGui::InputText("String", buf, 128, flags);
        component.text = std::string(buf);
    });

    drawComponent<MeshComponent>("Mesh", entity, [&entity](auto& component)
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
            FileSelectWindow::open(&component);
        }

        memset(buf, 0, 128);
        strcpy(buf, std::to_string(component.meshID).c_str());
        flags = ImGuiInputTextFlags_EnterReturnsTrue;
        if (ImGui::InputText("ID", buf, 128, flags))
        {
            int meshID = std::stoi(std::string(buf));
            component.meshID = meshID;
        }


        if (FileSelectWindow::selectFile(&component, "Choose mesh...", ".obj", ".fbx", ".blend", ".3ds"))
        {
            if (!FileSelectWindow::display())
            {
                if (FileSelectWindow::madeSelection())
                {
                    component.filePath = FileSelectWindow::getSelection();
                    Shared<Model> model = Model::loadModel(component.filePath);

                    if (model->meshes.size() < component.meshID && component.meshID > 0)
                    {
                        if (model->meshes.size() > 0)
                        {
                            component.mesh = model->meshes[component.meshID];
                            
                            std::string name = std::string("material_") + std::to_string(Assets::getAssetCount<Material>());
                            Assets::add<Material>(name, model->meshes[component.meshID]->materials[0]);

                            if (entity.hasComponent<MeshRendererComponent>())
                            {
                                entity.getComponent<MeshRendererComponent>().materials.push_back(model->meshes[component.meshID]->materials[0]);
                            }
                        }
                    }
                }
            }
        }
    });

    drawComponent<SkyLightComponent>("Sky Light", entity, [](auto& component)
    {
        ImGui::PushID("Skylight");
        ImGui::DragFloat("Intensity", &component.intensity, 0.01f, 0.f);
        ImGui::PopID();
    });

    drawComponent<DirectionalLightComponent>("Directional Light", entity, [](auto& component)
    {
        ImGui::PushID("Directionallight");
        ImGui::ColorEdit3("Radiance", &component.radiance.x);
        ImGui::DragFloat("Intensity", &component.intensity, 0.01f, 0.f);
        ImGui::PopID();
    });

    drawComponent<PointLightComponent>("Point Light", entity, [](auto& component)
    {
        ImGui::PushID("Pointlight");
        ImGui::ColorEdit4("Radiance", &component.radiance.x);
        ImGui::DragFloat("Intensity", &component.intensity, 0.01f, 0.f);
        ImGui::DragFloat("Attenuation", &component.attenuation, 0.01f, 0.f);
        ImGui::PopID();
    });

    drawComponent<MeshRendererComponent>("Mesh Renderer", entity, [](auto& component)
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
                    unsigned int materialCount = std::stoi(buf);

                    if (materialCount > 10)
                        materialCount = 10;

                    if (materialCount > component.materials.size())
                    {
                        auto material = Material::create();
                        std::string key = std::string("material_") + std::to_string(Assets::getAssetCount<Material>() - 1);
                        Assets::add<Material>(key, Material::create());
                        component.materials.insert(component.materials.end(), materialCount - component.materials.size(), material);
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
                for (auto& asset : Assets::getList<Material>()->getInternalList())
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
}

template<typename T, typename F>
void SceneHierarchy::drawComponent(const std::string& name, SceneEntity& entity, const F& func)
{
    if (!entity.hasComponent<T>())
    {
        return;
    }

    auto& component = entity.getComponent<T>();

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
        entity.removeComponent<T>();
    }
}

void SceneHierarchy::textureSelect(Shared<Texture2D>& texture)
{
    if (ImGui::Button("..."))
    {
        FileSelectWindow::open(&texture);
    }

    if (FileSelectWindow::selectFile(&texture, "Choose texture...", ".png", ".jpg", ".jpeg", ".tga", ".bmp", ".pic"))
    {
        if (!FileSelectWindow::display())
        {
            if (FileSelectWindow::madeSelection())
            {
                texture = Texture2D::create(FileSelectWindow::getSelection());
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
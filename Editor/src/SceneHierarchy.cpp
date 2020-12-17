#include "SceneHierarchy.h"

#include <imgui/imgui.h>

#include <scene/Components.h>
#include <scene/SceneEntity.h>
#include <renderer/Model.h>

SceneHierarchy::SceneHierarchy(const Shared<Scene>& scene)
    : m_context(scene)
{

}

void SceneHierarchy::onImGuiRender()
{
    ImGui::Begin("Scene Hierarchy");

    SceneEntity deletedEntity;
    m_context->getRegistry().each([&](Entity* entityHandle)
    {
        ImGui::PushID(entityHandle);

        SceneEntity entity = { entityHandle, m_context.get() };
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;
        bool opened = ImGui::TreeNodeEx(entity.getComponent<TagComponent>().tag.c_str(), flags);

        bool deleted = false;
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Delete Entity"))
            {
                deleted = true;
            }

            ImGui::EndPopup();
        }

        if (ImGui::IsItemClicked())
        {
            m_selection = entity;
        }

        if (opened)
        {
            ImGui::TreePop();
        }
        
        if (deleted)
        {
            m_selection = SceneEntity::createNull(m_context.get());
            deletedEntity = entity;
        }

        ImGui::PopID();
    });

    if (deletedEntity)
    {
        m_context->destroyEntity(deletedEntity);
    }

    if (ImGui::BeginPopupContextWindow(0, 1, false))
    {
        if (ImGui::MenuItem("Create Empty Entity"))
        {
            auto entity = m_context->createEntity("Untitled Entity");
            m_selection = entity;
        }

        ImGui::EndPopup();
    }

    ImGui::End();

    ImGui::Begin("Properties");

    if (m_selection)
    {
        drawProperties(m_selection);
    }

    ImGui::End();

    ImGui::Begin("Materials");

    if (m_selection)
    {
        if (m_selection.hasComponent<MeshComponent>())
        {
            drawMaterials(m_selection);
        }
    }

    ImGui::End();

    drawSceneRenderer();

    ImGui::Begin("Debug");

    ImGui::End();

    ImGui::Begin("Environment");

    float exposure;
    ImGui::SliderFloat("Exposure", &exposure, 0.f, 5.f);

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
    ImGui::InputText("Name", buf, 128);
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

    drawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
    {
        ImGui::ColorEdit4("Color", &(component.color.x));

        if (component.texture == nullptr)
        {
            component.texture = Texture2D::create(1, 1);
            uint32_t white = 0xffffffff;
            component.texture->setData(0, 0, 1, 1, &white);
        }

        char buf[128];
        strcpy(buf, component.texture->getPath().c_str());

        ImGui::InputText("Texture Path", buf, 128);

        if (std::string(buf) != component.texture->getPath())
        {
            component.texture = Texture2D::create(std::string(buf));
        }

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
        ImGui::InputText("String", buf, 128);
        component.text = std::string(buf);
    });

    drawComponent<MeshComponent>("Mesh", entity, [](auto& component)
    {
        char buf[128];
        strcpy(buf, component.filePath.c_str());
        if (ImGui::InputText("File Path", buf, 128))
            component.filePath = std::string(buf);

        // TODO: refactor mesh/model system
        if (ImGui::Button("Load"))
        {
            Shared<Model> model = Model::loadModel(component.filePath);
            component.mesh = model->meshes[0];
            component.materials.push_back(component.mesh->materials[0]);
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
}

template<typename T, typename F>
void SceneHierarchy::drawComponent(const std::string& name, SceneEntity& entity, const F& func)
{
    if (!entity.hasComponent<T>())
    {
        return;
    }

    auto& component = entity.getComponent<T>();

    float lineHeight = ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.f;
    auto available = ImGui::GetContentRegionAvail();

    ImGui::Separator();

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;
    flags |= ImGuiTreeNodeFlags_CollapsingHeader;
    bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, name.c_str());

    /*ImGui::SameLine(available.x - lineHeight * 0.5f);
    if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }));
    {
        std::cout << "Clicked\n";
        ImGui::OpenPopup("");
    }

    bool deleted = false;
    if (ImGui::BeginPopup(""))
    {
        if (ImGui::MenuItem("Remove Component"))
        {
            deleted = true;
        }

        ImGui::EndPopup();
    }*/

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
    char buf[128];
    strcpy(buf, texture->getPath().c_str());
    ImGui::InputText("Texture", buf, 128);

    if (ImGui::Button("Load"))
    {
        texture = Texture2D::create(std::string(buf));
    }
}

void SceneHierarchy::drawMaterials(SceneEntity& entity)
{
    for (auto& material : entity.getComponent<MeshComponent>().materials)
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
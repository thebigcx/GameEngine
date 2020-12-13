#include "SceneHierarchy.h"

#include <imgui/imgui.h>

#include <scene/Components.h>

SceneHierarchy::SceneHierarchy(const Shared<Scene>& scene)
    : m_context(scene)
{

}

void SceneHierarchy::onImGuiRender()
{
    ImGui::Begin("Scene Hierarchy");

    m_context->getRegistry().each([&](Entity* entity)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;
        bool opened = ImGui::TreeNodeEx(entity->getRegistry()->get<TagComponent>(entity).tag.c_str(), flags);

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
            if (m_selection == entity)
            {
                m_selection = nullptr;
            }
            else
            {
                m_selection = entity;
            }
        }

        if (opened)
        {
            ImGui::TreePop();
        }

        if (deleted)
        {
            m_context->getRegistry().destroy(entity);
        }
    });

    if (ImGui::BeginPopupContextWindow(0, 1, false))
    {
        if (ImGui::MenuItem("Create Empty Entity"))
        {
            auto entity = m_context->getRegistry().create();
            m_context->getRegistry().emplace<TagComponent>(entity, "Untitled Entity");
        }

        ImGui::EndPopup();
    }

    ImGui::End();

    ImGui::Begin("Properties");

    m_context->getRegistry().each([&](Entity* entity)
    {
        if (m_selection == entity)
        {
            drawProperties(entity);
        }
    });

    ImGui::End();

    ImGui::ShowDemoWindow();
    
}

void SceneHierarchy::drawProperties(Entity* entity)
{
    if (ImGui::Button("Add Component"))
    {
        ImGui::OpenPopup("AddComponent");
    }

    if (ImGui::BeginPopup("AddComponent"))
    {
        if (ImGui::MenuItem("Transform Component"))
        {
            m_context->getRegistry().emplace<TransformComponent>(entity);
            ImGui::CloseCurrentPopup();
        }

        if (ImGui::MenuItem("Sprite Renderer"))
        {
            m_context->getRegistry().emplace<SpriteRendererComponent>(entity);
            ImGui::CloseCurrentPopup();
        }

        if (ImGui::MenuItem("Camera Component"))
        {
            m_context->getRegistry().emplace<CameraComponent>(entity);
            ImGui::CloseCurrentPopup();
        }

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
            ImGui::DragFloat("Size", &size);
            camera.setOrthoSize(size);

            float near = camera.getOrthoNear();
            ImGui::DragFloat("Near", &near);
            camera.setOrthoNear(near);

            float far = camera.getOrthoFar();
            ImGui::DragFloat("Far", &far);
            camera.setOrthoFar(far);
        }
        else if (camera.getProjectionType() == ProjectionType::Perspective)
        {
            float fov = camera.getPerspectiveFov();
            ImGui::DragFloat("FOV", &fov);
            camera.setPerspectiveFov(fov);

            float near = camera.getPerspectiveNear();
            ImGui::DragFloat("Near", &near);
            camera.setPerspectiveNear(near);

            float far = camera.getPerspectiveFar();
            ImGui::DragFloat("Far", &far);
            camera.setPerspectiveFar(far);
        }
    });

    drawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
    {
        ImGui::ColorEdit4("Color", &(component.color.x));
    });
}

template<typename T, typename F>
void SceneHierarchy::drawComponent(const std::string& name, Entity* entity, const F& func)
{
    if (!entity->getRegistry()->has<T>(entity))
    {
        return;
    }

    auto& component = entity->getRegistry()->get<T>(entity);

    float lineHeight = ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.f;
    auto available = ImGui::GetContentRegionAvail();

    ImGui::Separator();

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;
    flags |= ImGuiTreeNodeFlags_CollapsingHeader;
    bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, name.c_str());

    ImGui::SameLine(available.x - lineHeight * 0.5f);
    if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }));
    {
        //ImGui::OpenPopup("ComponentSettings");
    }

    bool deleted = false;
    if (ImGui::BeginPopup("ComponentSettings"))
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
        m_context->getRegistry().remove<T>(entity);
    }
}
#include "SceneHierarchy.h"

#include <imgui/imgui.h>

#include <scene/Components.h>
#include <scene/SceneEntity.h>

SceneHierarchy::SceneHierarchy(const Shared<Scene>& scene)
    : m_context(scene)
{

}

void SceneHierarchy::onImGuiRender()
{
    ImGui::Begin("Scene Hierarchy");

    m_context->getRegistry().each([&](Entity* entityHandle)
    {
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
            m_context->destroyEntity(entity);
        }
    });

    if (ImGui::BeginPopupContextWindow(0, 1, false))
    {
        if (ImGui::MenuItem("Create Empty Entity"))
        {
            m_context->createEntity("Untitled Entity");
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
}

void SceneHierarchy::drawProperties(SceneEntity& entity)
{
    if (ImGui::Button("Add Component"))
    {
        ImGui::OpenPopup("AddComponent");
    }

    if (ImGui::BeginPopup("AddComponent"))
    {
        if (ImGui::MenuItem("Transform Component"))
        {
            entity.addComponent<TransformComponent>();
            ImGui::CloseCurrentPopup();
        }

        if (ImGui::MenuItem("Sprite Renderer"))
        {
            entity.addComponent<SpriteRendererComponent>();
            ImGui::CloseCurrentPopup();
        }

        if (ImGui::MenuItem("Camera Component"))
        {
            entity.addComponent<CameraComponent>();
            ImGui::CloseCurrentPopup();
        }

        if (ImGui::MenuItem("Text Renderer"))
        {
            entity.addComponent<TextRendererComponent>();
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
        //ImGui::Text("File selection window goes here...");
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
        ImGui::DragFloat("Width", &component.textureRect.width);
        ImGui::DragFloat("Height", &component.textureRect.height);
        
    });

    drawComponent<TextRendererComponent>("Text Renderer", entity, [](auto& component)
    {
        ImGui::ColorEdit4("Color", &component.color.x);
        ImGui::Text("Font selection window goes here...");

        char buf[128];
        ImGui::InputText("String", buf, 128);
        component.text = std::string(buf);
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
        entity.removeComponent<T>();
    }
}
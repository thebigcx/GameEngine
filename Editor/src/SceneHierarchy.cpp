#include "SceneHierarchy.h"

#include <imgui/imgui.h>

#include <scene/Components.h>

SceneHierarchy::SceneHierarchy(const Shared<Scene>& scene)
    : m_context(scene)
{

}

void SceneHierarchy::onImGuiRender()
{
    ImGui::Begin("Scene Heirarchy");

    

    m_context->getRegistry().each([&](Entity* entity)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;
        bool opened = ImGui::TreeNodeEx(entity->getRegistry()->get<TagComponent>(entity).tag.c_str(), flags);

        if (opened)
        {   
            ImGui::Button("Add Component");
            drawComponent<TransformComponent>("Transform", entity, [](auto& component)
            {
                ImGui::Text("Transform goes here...");
            });

            drawComponent<CameraComponent>("Camera", entity, [](auto& component)
            {
                ImGui::Checkbox("Primary", &component.primary);

                auto& camera = component.camera;

                if (camera.getProjectionType() == ProjectionType::Orthographic)
                {
                    float orthoNear;
                    if (ImGui::DragFloat("Near", &orthoNear))
                    {

                    }

                    float orthoFar;
                    if (ImGui::DragFloat("Far", &orthoFar))
                    {
                        
                    }
                }
                else if (camera.getProjectionType() == ProjectionType::Perspective)
                {

                }
            });

            drawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
            {
                ImGui::ColorEdit4("Color", &(component.color.x));
            });

            ImGui::TreePop();
        }
    });

    ImGui::End();
}

template<typename T, typename F>
void SceneHierarchy::drawComponent(const std::string& name, Entity* entity, const F& func)
{
    if (!entity->getRegistry()->has<T>(entity))
    {
        return;
    }

    auto& component = entity->getRegistry()->get<T>(entity);

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;
    bool opened = ImGui::TreeNodeEx(name.c_str(), flags);

    if (opened)
    {
        func(component);
        ImGui::TreePop();
    }
}
#pragma once

#include <core/Layer.h>
#include <scene/Scene.h>
#include <scene/ecs/Registry.h>

class EditorLayer : public Layer
{
public:
    EditorLayer();

    void onAttach() override;
    void onDetach() override;
    void onUpdate() override;
    void onImGuiRender() override;
    void onEvent(Event& event) override;

private:
    //Scene m_scene;
    EntityRegistry m_registry;
};
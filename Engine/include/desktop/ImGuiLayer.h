#pragma once

#include <core/Layer.h>

class ImGuiLayer : public Layer
{
public:
    ImGuiLayer();

    void onAttach() override;
    void onDetach() override;
    void onUpdate(float dt) override;
    void onEvent(Event& event) override;

    void begin();
    void end();

private:
    bool m_blockEvents = false;
};
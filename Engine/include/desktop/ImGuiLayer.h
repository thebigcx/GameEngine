#pragma once

#include <core/Layer.h>

namespace Engine
{

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

    void blockEvents(bool block = true);

private:
    bool m_blockEvents = false;
};

}
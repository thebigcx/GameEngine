#pragma once

#include <core/Layer.h>

class EditorLayer : public Layer
{
public:
    EditorLayer();

    void onAttach() override;
    void onDetach() override;
    void onUpdate() override;
    void onEvent(Event& event) override;

private:
    bool m_blockEvents = false;
};
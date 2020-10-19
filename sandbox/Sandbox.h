#pragma once

#include <core/Layer.h>
#include <renderer/Texture.h>

class Sandbox : public Layer
{
public:
    Sandbox();

    void update() override;
    void handleEvent(const Event& event) override;

private:
    Texture m_texture;

    Batch m_batch;
    Quad m_quad;
};
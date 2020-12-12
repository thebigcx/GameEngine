#pragma once

#include <maths/vector/vec3.h>
#include <maths/matrix/mat4x4.h>

#include <events/Event.h>
#include <events/EventDispatcher.h>

#include <util/PerspectiveCamera.h>

class PerspectiveCameraController : public PerspectiveCamera
{
public:
    PerspectiveCameraController();

    void update(float dt);

private:
    float m_speed = 0.1f;

    math::ivec2 m_lastMousePos;

    float m_sensitivity = 0.1f;
};
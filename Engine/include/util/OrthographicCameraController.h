#pragma once

#include <maths/vector/vec2.h>
#include <maths/matrix/mat4x4.h>

#include <events/Event.h>
#include <events/EventDispatcher.h>

#include <util/OrthographicCamera.h>

class OrthographicCameraController : public OrthographicCamera
{
public:
    OrthographicCameraController();

    void update();

private:
    float m_speed = 1.f;
};
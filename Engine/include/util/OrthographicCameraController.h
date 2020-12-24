#pragma once

#include <maths/vector/vec2.h>
#include <maths/matrix/mat4x4.h>

#include <events/Event.h>
#include <events/EventDispatcher.h>

#include <util/OrthographicCamera.h>

namespace Engine
{

class OrthographicCameraController : public OrthographicCamera
{
public:
    OrthographicCameraController() = default;

    void update(float dt);


    void setSpeed(float speed)
    {
        m_speed = speed;
    }

    inline constexpr float getSpeed() const noexcept
    {
        return m_speed;
    }

private:
    float m_speed = 1.f;
};

}
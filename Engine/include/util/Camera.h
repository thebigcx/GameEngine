#pragma once

#include <maths/matrix/mat4x4.h>

namespace Engine
{

class Camera
{
public:
    Camera() = default;
    virtual ~Camera() = default;

    inline constexpr const math::mat4& getProjectionMatrix() const noexcept
    {
        return m_projectionMatrix;
    }

    const math::mat4& getViewMatrix()
    {
        return m_viewMatrix;
    }

protected:
    math::mat4 m_projectionMatrix = math::mat4(1.f);
    math::mat4 m_viewMatrix = math::mat4(1.f);
};

}
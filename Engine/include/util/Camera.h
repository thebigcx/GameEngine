#pragma once

#include <maths/matrix/mat4x4.h>

namespace Engine
{

class Camera
{
public:
    Camera() = default;
    virtual ~Camera() = default;

    inline constexpr const math::mat4& getProjection() const noexcept
    {
        return m_projection;
    }

protected:
    math::mat4 m_projection = math::mat4(1.f);
};

}
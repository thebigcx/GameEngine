#pragma once

#include <maths/math.h>

#include <util/Camera.h>

namespace Engine
{

class Event;
class WindowResizeEvent;

class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera();
    virtual ~PerspectiveCamera() = default;

    virtual void setPosition(const math::vec3& position);
    virtual void setPosition(float x, float y, float z);

    virtual void setDirection(const math::vec3& direction);
    virtual void setDirection(float x, float y, float z);

    virtual void setRenderDistance(float renderDistance);
    virtual void setFieldOfView(float fov);

    inline constexpr const math::vec3& getPosition() const noexcept { return m_position; }
    inline constexpr const math::vec3& getDirection() const noexcept { return m_direction; }
    inline constexpr float getRenderDistance() const noexcept { return m_renderDistance; }
    inline constexpr float getFieldOfView() const noexcept { return m_fieldOfView; }

    void setViewportSize(uint32_t width, uint32_t height);

protected:
    void recalculateView();

protected:
    math::vec3 m_position;
    math::vec3 m_direction;
    math::vec3 m_up;

    float m_pan = 0;
    float m_tilt = 0;

    float m_fieldOfView = 45.f;
    float m_renderDistance = 100.f;
};

}
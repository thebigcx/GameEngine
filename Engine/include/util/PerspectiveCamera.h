#pragma once

#include <maths/math.h>

#include <events/Event.h>
#include <events/EventDispatcher.h>

class PerspectiveCamera
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

    math::mat4 getViewMatrix() const;

    inline constexpr const math::mat4& getProjectionMatrix() const
    {
        return m_projectionMatrix;
    }

    virtual void onEvent(Event& event);
    virtual void onWindowResize(WindowResizeEvent& event);

protected:
    math::vec3 m_position;
    math::vec3 m_direction;
    math::vec3 m_up;

    math::mat4 m_projectionMatrix;

    float m_pan = 0;
    float m_tilt = 0;

    float m_fieldOfView = 45.f;
    float m_renderDistance = 100.f;
};
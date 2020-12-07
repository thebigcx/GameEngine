#pragma once

#include <maths/math.h>

#include <events/Event.h>
#include <events/EventDispatcher.h>

class PerspectiveCamera
{
public:
    PerspectiveCamera();

    void setPosition(const math::vec3& position);
    void setDirection(const math::vec3& direction);

    inline const math::vec3& getPosition() const { return m_position; };
    inline const math::vec3& getDirection() const { return m_direction; };

    math::mat4 getViewMatrix() const;

    void onEvent(Event& event);
    void onWindowResize(WindowResizeEvent& event);

    inline constexpr const math::mat4& getProjectionMatrix() const
    {
        return m_projectionMatrix;
    }

protected:
    math::vec3 m_position;
    math::vec3 m_direction;
    math::vec3 m_up;

    float m_pan = 0;
    float m_tilt = 0;

    math::mat4 m_projectionMatrix;

    float m_fieldOfView = 45.f;
    float m_renderDistance = 100.f;
};
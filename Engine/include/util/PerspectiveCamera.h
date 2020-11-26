#pragma once

#include <maths/vector/vec3.h>
#include <maths/matrix/mat4.h>

class PerspectiveCamera
{
public:
    PerspectiveCamera();

    void setPosition(const math::vec3& position);
    void setDirection(const math::vec3& direction);

    void update();

    inline const math::vec3& getPosition() const { return m_position; };
    inline const math::vec3& getDirection() const { return m_direction; };

    math::mat4 getViewMatrix() const;

private:
    math::vec3 m_position;
    math::vec3 m_direction;
    math::vec3 m_up;
    float m_speed = 0.1f;

    float m_pan = 0;
    float m_tilt = 0;

    math::ivec2 m_lastMousePos;

    float m_sensitivity = 0.1f;
};
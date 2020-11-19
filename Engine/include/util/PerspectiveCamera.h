#pragma once

#include <math/vector/vec3.h>
#include <math/matrix/mat4.h>

class PerspectiveCamera
{
public:
    PerspectiveCamera();

    void setPosition(const math::vec3& position);
    void setDirection(const math::vec3& direction);

    inline const math::vec3& getPosition() const { return m_position; };
    inline const math::vec3& getDirection() const { return m_direction; };

    math::mat4 getViewMatrix() const;

private:
    math::vec3 m_position;
    math::vec3 m_direction;
    math::vec3 m_up;
};
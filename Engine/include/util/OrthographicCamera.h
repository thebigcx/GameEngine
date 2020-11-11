#pragma once

#include <math/vector/Vector2.h>
#include <math/matrix/Matrix4.h>

class OrthographicCamera
{
public:
    OrthographicCamera() {}
    OrthographicCamera(const math::Vector2f& position);

    void setPosition(const math::Vector2f& position);
    void translate(const math::Vector2f& vec);

    inline const math::Vector2f& getPosition() const { return m_position; }

    math::Matrix4f getViewMatrix() const;

private:
    math::Vector2f m_position;
};
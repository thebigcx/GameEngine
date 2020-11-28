#pragma once

#include <maths/vector/vec2.h>
#include <maths/matrix/mat4x4.h>

class OrthographicCamera
{
public:
    OrthographicCamera() {}
    OrthographicCamera(const math::vec2& position);

    void setPosition(const math::vec2& position);
    void translate(const math::vec2& vec);

    inline const math::vec2& getPosition() const { return m_position; }

    math::mat4 getViewMatrix() const;

private:
    math::vec2 m_position;
};
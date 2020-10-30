#pragma once

#include <math/vector/Vector2.h>
#include <math/matrix/Matrix4.h>

class OrthographicCamera
{
public:
    OrthographicCamera() {}
    OrthographicCamera(const Vector2f& position);

    void setPosition(const Vector2f& position);
    void translate(const Vector2f& vec);

    inline const Vector2f& getPosition() const { return m_position; }

    Matrix4f getViewMatrix() const;

private:
    Vector2f m_position;
};
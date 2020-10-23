#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <util/maths/Vector2.h>

class Camera
{
public:
    Camera() {}
    Camera(const Vector2f& position);

    void setPosition(const Vector2f& position);
    void translate(const Vector2f& vec);

    inline const Vector2f& getPosition() const { return m_position; }

    glm::mat4 getViewMatrix() const;

private:
    Vector2f m_position;
};
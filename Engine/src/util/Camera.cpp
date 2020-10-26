#include <util/Camera.h>

Camera::Camera(const Vector2f& position)
    : m_position(position)
{

}

void Camera::setPosition(const Vector2f& position)
{
    m_position = position;
}

void Camera::translate(const Vector2f& vec)
{
    m_position += vec;
}

glm::mat4 Camera::getViewMatrix() const
{
    glm::mat4 matrix(1.f);

    matrix = glm::lookAt(glm::vec3(m_position.x, m_position.y, 1), glm::vec3(m_position.x, m_position.y, 0), glm::vec3(0, 1, 0));

    return matrix;
}
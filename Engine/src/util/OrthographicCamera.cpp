#include <util/OrthographicCamera.h>

#include <maths/matrix/matrix_transform.h>
#include <iostream>

OrthographicCamera::OrthographicCamera()
{
    m_projectionMatrix = math::ortho(0.f, 1280.f, 0.f, 720.f, -1.f, 1.f);
}

void OrthographicCamera::setPosition(const math::vec2& position)
{
    m_position = position;
}

void OrthographicCamera::translate(const math::vec2& vec)
{
    m_position += vec;
}

math::mat4 OrthographicCamera::getViewMatrix() const
{
    return math::lookAt(math::vec3(m_position, 1), math::vec3(m_position, -1), math::vec3(0, 1, 0));
}
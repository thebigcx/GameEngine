#include <util/OrthographicCamera.h>

#include <math/matrix/MatrixTransform.h>
#include <iostream>

OrthographicCamera::OrthographicCamera(const math::Vector2f& position)
    : m_position(position)
{

}

void OrthographicCamera::setPosition(const math::Vector2f& position)
{
    m_position = position;
}

void OrthographicCamera::translate(const math::Vector2f& vec)
{
    m_position += vec;
}

math::Matrix4f OrthographicCamera::getViewMatrix() const
{
    return math::lookAt(math::Vector3f(m_position, -1), math::Vector3f(m_position, 1), math::Vector3f(0, 1, 0));
}
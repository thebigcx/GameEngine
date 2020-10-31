#include <util/OrthographicCamera.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

OrthographicCamera::OrthographicCamera(const Vector2f& position)
    : m_position(position)
{

}

void OrthographicCamera::setPosition(const Vector2f& position)
{
    m_position = position;
}

void OrthographicCamera::translate(const Vector2f& vec)
{
    m_position += vec;
}

Matrix4f OrthographicCamera::getViewMatrix() const
{
    return Matrix4f::createOrthoCamera(Vector3f(m_position, -1));
}
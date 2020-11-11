#include <util/OrthographicCamera.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
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
    return math::Matrix4f::createOrthoView(math::Vector3f(m_position, -1));
}
#include <util/OrthographicCamera.h>
#include <maths/matrix/matrix_transform.h>

namespace Engine
{

OrthographicCamera::OrthographicCamera()
{
    m_projectionMatrix = math::ortho(0.f, 1280.f, 0.f, 720.f, -1.f, 1.f);
}

void OrthographicCamera::setPosition(const math::vec3& position)
{
    m_position = position;
    recalculateView();
}

void OrthographicCamera::setPosition(float x, float y, float z)
{
    m_position = { x, y, z };
    recalculateView();
}

void OrthographicCamera::translate(const math::vec3& vec)
{
    m_position += vec;
    recalculateView();
}

void OrthographicCamera::translate(float x, float y, float z)
{
    m_position += { x, y, z };
    recalculateView();
}

void OrthographicCamera::setRotation(float rotation)
{
    m_rotation = rotation;
    recalculateView();
}

void OrthographicCamera::rotate(float rotation)
{
    m_rotation += rotation;
    recalculateView();
}

void OrthographicCamera::recalculateView()
{
    math::mat4 transform = math::translate(math::mat4(1.f), m_position) *
                           math::rotate(math::mat4(1.f), (float)math::radians(m_rotation), math::vec3(0, 0, 1));

    m_viewMatrix = math::inverse(transform);
}

void OrthographicCamera::setProjection(uint32_t left, uint32_t right, uint32_t bottom, uint32_t top)
{
    m_projectionMatrix = math::ortho((float)left, (float)right, (float)right, (float)top, -1.f, 1.f);
}

}
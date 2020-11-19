#include <util/PerspectiveCamera.h>
#include <math/matrix/matrix_transform.h>

PerspectiveCamera::PerspectiveCamera()
    : m_up(0, 1, 0), m_direction(0, -1, 0)
{

}

void PerspectiveCamera::setPosition(const math::vec3& position)
{
    m_position = position;
}

void PerspectiveCamera::setDirection(const math::vec3& direction)
{
    m_direction = direction;
}

math::mat4 PerspectiveCamera::getViewMatrix() const
{
    return math::lookAt(m_position, m_direction, m_up);
}
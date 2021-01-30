#include <util/PerspectiveCamera.h>

namespace Engine
{

PerspectiveCamera::PerspectiveCamera()
    : m_up(0, 1, 0), m_direction(0, 0, -1)
{
    m_projectionMatrix = math::perspective((float)math::radians(m_fieldOfView), 1280.f / 720.f, 0.1f, m_renderDistance);
}

void PerspectiveCamera::setPosition(const math::vec3& position)
{
    m_position = position;
    recalculateView();
}

void PerspectiveCamera::setPosition(float x, float y, float z)
{
    m_position = { x, y, z };
    recalculateView();
}

void PerspectiveCamera::setDirection(const math::vec3& direction)
{
    m_direction = direction;
    recalculateView();
}

void PerspectiveCamera::setDirection(float x, float y, float z)
{
    m_direction = { x, y, z };
    recalculateView();
}

void PerspectiveCamera::setRenderDistance(float renderDistance)
{
    m_renderDistance = renderDistance;
}

void PerspectiveCamera::setFieldOfView(float fov)
{
    m_fieldOfView = fov;
}

void PerspectiveCamera::recalculateView()
{
    m_viewMatrix = math::lookAt(m_position, m_position + m_direction, m_up);
}

void PerspectiveCamera::setViewportSize(uint32_t width, uint32_t height)
{
    m_projectionMatrix = math::perspective((float)math::radians(m_fieldOfView), (float)width / (float)height, 0.1f, m_renderDistance);
}

}
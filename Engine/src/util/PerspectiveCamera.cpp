#include <util/PerspectiveCamera.h>

PerspectiveCamera::PerspectiveCamera()
    : m_up(0, 1, 0), m_direction(0, 0, -1)
{
    m_projectionMatrix = math::perspective((float)math::radians(m_fieldOfView), 1280.f / 720.f, 0.1f, m_renderDistance);
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
    math::mat4 view = math::lookAt(m_position, m_position + m_direction, m_up);

    return view;
}

void PerspectiveCamera::onEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<WindowResizedEvent>(BIND_EVENT_FN(onWindowResize));
}

void PerspectiveCamera::onWindowResize(WindowResizedEvent& event)
{
    m_projectionMatrix = math::perspective((float)math::radians(m_fieldOfView), (float)event.getWidth() / (float)event.getHeight(), 0.1f, m_renderDistance);
}
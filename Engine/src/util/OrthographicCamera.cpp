#include <util/OrthographicCamera.h>

namespace Engine
{

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

void OrthographicCamera::translate(float x, float y)
{
    m_position += math::vec2(x, y);
}

math::mat4 OrthographicCamera::getViewMatrix() const
{
    return math::lookAt(math::vec3(m_position, 1), math::vec3(m_position, -1), math::vec3(0, 1, 0));
}

void OrthographicCamera::onEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(onWindowResize));
}

void OrthographicCamera::onWindowResize(WindowResizeEvent& event)
{
    m_projectionMatrix = math::ortho(0.f, (float)event.getWidth(), 0.f, (float)event.getHeight(), -1.f, 1.f);
}

}
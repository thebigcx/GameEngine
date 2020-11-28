#include <util/PerspectiveCamera.h>
#include <maths/matrix/matrix_transform.h>
#include <core/Input.h>

PerspectiveCamera::PerspectiveCamera()
    : m_up(0, 1, 0), m_direction(0, 0, -1)
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
    math::mat4 view = math::lookAt(m_position, m_position + m_direction, m_up);

    return view;
}

void PerspectiveCamera::update()
{
    if (Input::isKeyPressed(Key::W))
        m_position += math::vec3(cos(math::radians(m_pan)), 0, sin(math::radians(m_pan))) * m_speed;
    
    if (Input::isKeyPressed(Key::S))
        m_position -= math::vec3(cos(math::radians(m_pan)), 0, sin(math::radians(m_pan))) * m_speed;

    if (Input::isKeyPressed(Key::A))
        m_position -= math::normalize(math::cross(m_direction, m_up)) * m_speed;

    if (Input::isKeyPressed(Key::D))
        m_position += math::normalize(math::cross(m_direction, m_up)) * m_speed;

    if (Input::isKeyPressed(Key::Space))
        m_position.y += m_speed;

    if (Input::isKeyPressed(Key::LeftShift))
        m_position.y -= m_speed;

    math::ivec2 mousePos = Input::getMousePosition();

    float xoffset = mousePos.x - m_lastMousePos.x;
    float yoffset = mousePos.y - m_lastMousePos.y;

    m_pan += xoffset * m_sensitivity;
    m_tilt += yoffset * m_sensitivity;
    m_tilt = math::clamp(m_tilt, -89.5f, 89.5f);

    m_lastMousePos = mousePos;

    m_direction.x = math::cos(math::radians(m_pan)) * math::cos(math::radians(m_tilt));
    m_direction.y = math::sin(math::radians(m_tilt));
    m_direction.z = math::sin(math::radians(m_pan)) * math::cos(math::radians(m_tilt));

    m_direction = math::normalize(m_direction);
}
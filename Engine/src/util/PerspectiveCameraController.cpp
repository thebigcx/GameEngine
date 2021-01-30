#include <util/PerspectiveCameraController.h>
#include <maths/matrix/matrix_transform.h>

#include <core/Keyboard.h>
#include <core/Mouse.h>

namespace Engine
{

PerspectiveCameraController::PerspectiveCameraController()
{

}

void PerspectiveCameraController::update(float dt)
{
    if (Keyboard::isPressed(Keyboard::Key::W))
        m_position += math::vec3(cos(math::radians(m_pan)), 0, sin(math::radians(m_pan))) * m_speed * dt;
    
    if (Keyboard::isPressed(Keyboard::Key::S))
        m_position -= math::vec3(cos(math::radians(m_pan)), 0, sin(math::radians(m_pan))) * m_speed * dt;

    if (Keyboard::isPressed(Keyboard::Key::A))
        m_position -= math::normalize(math::cross(m_direction, m_up)) * m_speed * dt;

    if (Keyboard::isPressed(Keyboard::Key::D))
        m_position += math::normalize(math::cross(m_direction, m_up)) * m_speed * dt;

    if (Keyboard::isPressed(Keyboard::Key::Space))
        m_position.y += m_speed * dt;

    if (Keyboard::isPressed(Keyboard::Key::LeftShift))
        m_position.y -= m_speed * dt;

    math::ivec2 mousePos = Mouse::getPosition();

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

    recalculateView();
}

}
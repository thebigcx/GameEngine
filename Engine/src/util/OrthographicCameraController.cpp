#include <util/OrthographicCameraController.h>
#include <core/Keyboard.h>

namespace Engine
{

void OrthographicCameraController::update(float dt)
{
    if (Keyboard::isPressed(Keyboard::Key::W))
    {
        this->translate(0.f, m_speed * dt, 0.f);
    }
    if (Keyboard::isPressed(Keyboard::Key::A))
    {
        this->translate(-m_speed * dt, 0.f, 0.f);
    }
    if (Keyboard::isPressed(Keyboard::Key::S))
    {
        this->translate(0.f, -m_speed * dt, 0.f);
    }
    if (Keyboard::isPressed(Keyboard::Key::D))
    {
        this->translate(m_speed * dt, 0.f, 0.f);
    }
}

}
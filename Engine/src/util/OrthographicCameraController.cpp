#include <util/OrthographicCameraController.h>
#include <core/Input.h>

namespace Engine
{

void OrthographicCameraController::update(float dt)
{
    if (Input::isKeyPressed(Key::W))
    {
        this->translate(0.f, m_speed * dt);
    }
    if (Input::isKeyPressed(Key::A))
    {
        this->translate(-m_speed * dt, 0.f);
    }
    if (Input::isKeyPressed(Key::S))
    {
        this->translate(0.f, -m_speed * dt);
    }
    if (Input::isKeyPressed(Key::D))
    {
        this->translate(m_speed * dt, 0.f);
    }
}

}
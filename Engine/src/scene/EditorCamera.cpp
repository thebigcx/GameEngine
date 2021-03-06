#include <scene/EditorCamera.h>

#include <maths/matrix/matrix_func.h>
#include <maths/quaternion/qua_func.h>

#include <core/Keyboard.h>
#include <core/Mouse.h>

#include <events/EventDispatcher.h>
#include <events/MouseEvent.h>

namespace Engine
{

EditorCamera::EditorCamera(float fov, float aspect, float near, float far)
    : m_fov(fov), m_aspect(aspect), m_near(near), m_far(far)
{
    updateProjection();
}

void EditorCamera::onUpdate(float dt)
{
    
    math::vec2 mouse(Mouse::getPosition().x, Mouse::getPosition().y);
    math::vec2 delta = (mouse - m_lastMousePosition) * 0.003f;
    delta.y = -delta.y; // Flip y-axis
    m_lastMousePosition = mouse;

    if (Keyboard::isPressed(Keyboard::Key::LeftAlt))
    {
        if (Mouse::isPressed(Mouse::Button::Left))
        {
            mouseRotate(delta);
        }

        if (Mouse::isPressed(Mouse::Button::Right))
        {
            // Zoom
            mouseZoom(delta.y);
        }
    }

    if (Mouse::isPressed(Mouse::Button::Right))
    {
        // Rotate camera itself
    }

    if (Mouse::isPressed(Mouse::Button::Middle))
    {
        mousePan(delta);
    }

    updateView();
}

void EditorCamera::onEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<MouseScrollEvent>(BIND_EVENT_FN(EditorCamera::onMouseScroll));   
}

void EditorCamera::setViewportSize(uint32_t width, uint32_t height)
{
    m_viewportSize = { width, height };
    updateProjection();
}

math::vec3 EditorCamera::getRightDirection()
{
    return math::rotate(getOrientation(), math::vec3(1.f, 0.f, 0.f));
}

math::vec3 EditorCamera::getForwardDirection()
{
    return math::rotate(getOrientation(), math::vec3(0.f, 0.f, -1.f));
}

math::vec3 EditorCamera::getUpDirection()
{
    return math::rotate(getOrientation(), math::vec3(0.f, 1.f, 0.f));
}

void EditorCamera::updateView()
{
    m_position = getPosition();

    math::quat orientation = getOrientation();
    m_viewMatrix = math::translate(math::mat4(1.f), m_position) * math::to_mat4(orientation);
    m_viewMatrix = math::inverse<float>(m_viewMatrix);
}

void EditorCamera::updateProjection()
{
    m_aspect = static_cast<float>(m_viewportSize.x) / static_cast<float>(m_viewportSize.y);
    m_projectionMatrix = math::perspective(static_cast<float>(math::radians(m_fov)), m_aspect, m_near, m_far);
}

math::vec2 EditorCamera::panSpeed() const
{
    float x = std::min(m_viewportSize.x / 1000.f, 2.4f);
    float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

    float y = std::min(m_viewportSize.y / 1000.f, 2.4f);
    float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

    return math::vec2(xFactor, yFactor);
}

float EditorCamera::zoomSpeed() const
{
    float dist = m_distance * 0.5f;
    dist = std::max(dist, 0.f);
    float speed = dist * dist;
    speed = std::min(speed, 100.f);
    return speed;
}

float EditorCamera::rotationSpeed() const
{
    return 0.5f;
}

void EditorCamera::mousePan(const math::vec2& delta)
{
    auto speed = panSpeed();

    m_focalPoint += -getRightDirection() * delta.x * speed.x * m_distance;
    m_focalPoint += getUpDirection() * delta.y * speed.y * m_distance;
}

void EditorCamera::mouseRotate(const math::vec2& delta)
{
    //float yawSign = getUpDirection().y < 0 ? -1.f : 1.f;
    float yawSign = 1.f;
    m_yaw += yawSign * delta.x * rotationSpeed();
    m_pitch += delta.y * rotationSpeed();
}

void EditorCamera::mouseZoom(float delta)
{
    m_distance -= delta * zoomSpeed();
    if (m_distance < 1.f)
    {
        m_focalPoint += getForwardDirection();
        m_distance = 1.f;
    }
}

bool EditorCamera::onMouseScroll(MouseScrollEvent& event)
{
    float delta = event.getYOffset() * 0.1f;
    mouseZoom(delta);
    return false;
}

math::quat EditorCamera::getOrientation()
{
    return math::quat(math::vec3(-m_pitch, -m_yaw, 0.f));
}

math::vec3 EditorCamera::getPosition()
{
    return m_focalPoint - getForwardDirection() * m_distance;
}

}
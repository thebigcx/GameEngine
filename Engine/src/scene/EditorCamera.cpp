#include <scene/EditorCamera.h>
#include <events/EventDispatcher.h>

EditorCamera::EditorCamera(float fov, float aspect, float near, float far)
    : m_fov(fov), m_aspect(aspect), m_near(near), m_far(far)
{
    updateProjection();
}

void EditorCamera::onUpdate(float dt)
{
    if (Input::isKeyPressed(Key::LeftAlt))
    {
        math::vec2 mouse(Input::getMousePosition().x, Input::getMousePosition().y);
        math::vec2 delta = (mouse - m_lastMousePosition) * 0.0003f;
        m_lastMousePosition = mouse;

        if (Input::isMousePressed(MouseButton::Left))
        {
            mouseRotate(delta);
        }

        if (Input::isMousePressed(MouseButton::Right))
        {

        }

        if (Input::isMousePressed(MouseButton::Middle))
        {
            mousePan(delta);
        }
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
    
}

void EditorCamera::updateView()
{
    //m_position = calculatePosition();


}

void EditorCamera::updateProjection()
{
    m_aspect = static_cast<float>(m_viewportSize.x) / static_cast<float>(m_viewportSize.y);
    m_projection = math::perspective(static_cast<float>(math::radians(m_fov)), m_aspect, m_near, m_far);
}

math::vec2 EditorCamera::panSpeed() const
{
    float x = std::min(m_viewportSize.x / 1000.f, 2.4f);
    float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

    float y = std::min(m_viewportSize.y / 1000.f, 2.4f);
    float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

    return math::vec2(xFactor, yFactor);
}

void EditorCamera::mousePan(const math::vec2& delta)
{
    auto speed = panSpeed();

    m_focalPoint += -getRightDirection() * delta.x * speed.x * m_distance;
    m_focalPoint += getRightDirection() * delta.y * speed.y * m_distance;
}

void EditorCamera::mouseRotate(const math::vec2& delta)
{

}

void EditorCamera::mouseZoom(float delta)
{

}

bool EditorCamera::onMouseScroll(MouseScrollEvent& event)
{
    float delta = event.getYOffset() * 0.1f;
    mouseZoom(delta);
    return false;
}
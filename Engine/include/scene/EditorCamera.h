#pragma once

#include <util/Camera.h>
#include <events/Event.h>

class EditorCamera : public Camera
{
public:
    EditorCamera() = default;
    EditorCamera(float fov, float aspect, float near, float far);

    void onUpdate(float dt);
    void onEvent(Event& event);

    void setViewportSize(uint32_t width, uint32_t height);

    math::vec3 getRightDirection();

    inline constexpr const math::mat4& getProjectionMatrix() const noexcept
    {
        return m_projection;
    }

    inline constexpr const math::mat4& getViewMatrix() const noexcept
    {
        return m_view;
    }

    math::mat4 getViewMatrix();

    math::vec2 panSpeed() const;

    void updateProjection();
    void updateView();

private:
    math::mat4 m_projection, m_view;

    float m_fov, m_aspect, m_near, m_far;

    math::vec2 m_viewportSize;

    float m_distance;

    math::vec3 m_position, m_focalPoint;
    math::vec3 m_up = math::vec3(0, 1, 0);

    float m_pan, m_tilt;

    math::vec2 m_lastMousePosition = { 0.f, 0.f };

private:
    bool onMouseScroll(MouseScrollEvent& event);

    void mousePan(const math::vec2& delta);
    void mouseRotate(const math::vec2& delta);
    void mouseZoom(float delta);
    
};
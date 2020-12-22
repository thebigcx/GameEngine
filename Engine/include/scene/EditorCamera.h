#pragma once

#include <util/Camera.h>
#include <events/Event.h>
#include <maths/quaternion/qua.h>

class EditorCamera : public Camera
{
public:
    EditorCamera() = default;
    EditorCamera(float fov, float aspect, float near, float far);

    void onUpdate(float dt);
    void onEvent(Event& event);

    void setViewportSize(uint32_t width, uint32_t height);

    float getNear() const { return m_near; }
    float getFar() const { return m_far; }
    void setNear(float near) { m_near = near; }
    void setFar(float far) { m_far = far; }

    math::vec3 getUpDirection();
    math::vec3 getForwardDirection();
    math::vec3 getRightDirection();

    inline constexpr const math::mat4& getProjectionMatrix() const noexcept
    {
        return m_projection;
    }

    inline constexpr const math::mat4& getViewMatrix() const noexcept
    {
        return m_view;
    }

    math::vec2 panSpeed() const;
    float zoomSpeed() const;
    float rotationSpeed() const;

    void updateProjection();
    void updateView();

    math::quat getOrientation();
    math::vec3 getPosition();

private:
    math::mat4 m_projection, m_view;

    float m_fov, m_aspect, m_near, m_far;

    math::uvec2 m_viewportSize;

    float m_distance = 10.f;

    math::vec3 m_position, m_focalPoint;
    math::vec3 m_up = math::vec3(0, 1, 0);

    float m_pitch = 0.f, m_yaw = 0.f;

    math::vec2 m_lastMousePosition = { 0.f, 0.f };

private:
    bool onMouseScroll(MouseScrollEvent& event);

    void mousePan(const math::vec2& delta);
    void mouseRotate(const math::vec2& delta);
    void mouseZoom(float delta);
    
};
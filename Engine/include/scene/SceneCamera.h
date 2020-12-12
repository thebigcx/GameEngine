#pragma once

#include <util/Camera.h>

enum class ProjectionType
{
    Perspective,
    Orthographic
};

class SceneCamera : public Camera
{
public:
    SceneCamera();

    inline constexpr const ProjectionType& getProjectionType() const noexcept
    {
        return m_projectionType;
    }

    void setOrthographic(float size, float near, float far);
    void setPerspective(float verticalFov, float near, float far);

    void calculateProjection();

    void setViewportSize(uint32_t width, uint32_t height);

private:
    ProjectionType m_projectionType = ProjectionType::Orthographic;

    float m_orthoSize = 10.f;
    float m_orthoNear = -1.f;
    float m_orthoFar = 1.f;

    float m_perspectiveFov = math::radians(45.f);
    float m_perspectiveNear = 0.01f;
    float m_perspectiveFar = 100.f;

    float m_aspect = 0.f;
};
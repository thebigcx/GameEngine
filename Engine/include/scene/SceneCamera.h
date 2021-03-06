#pragma once

#include <util/Camera.h>
#include <scene/GameComponent.h>

namespace Engine
{

class SceneCamera : public Camera, public GameComponent
{
public:
    SceneCamera();
    ~SceneCamera();

    void onSceneSet(const Reference<Scene>& scene) override;

    inline constexpr const Camera::ProjectionType& getProjectionType() const noexcept
    {
        return m_projectionType;
    }

    inline void setProjectionType(Camera::ProjectionType type)
    {
        m_projectionType = type;
        calculateProjection();
    }

    void setOrthographic(float size, float near, float far);
    void setPerspective(float verticalFov, float near, float far);

    void calculateProjection();

    void setViewportSize(uint32_t width, uint32_t height);

    float getOrthoSize() const { return m_orthoSize; }
    float getOrthoNear() const { return m_orthoNear; }
    float getOrthoFar() const { return m_orthoFar; }
    void setOrthoSize(float size) { m_orthoSize = size; calculateProjection(); }
    void setOrthoNear(float near) { m_orthoNear = near; calculateProjection(); }
    void setOrthoFar(float far) { m_orthoFar = far; calculateProjection(); }

    float getPerspectiveFov() const { return m_perspectiveFov; }
    float getPerspectiveNear() const { return m_perspectiveNear; }
    float getPerspectiveFar() const { return m_perspectiveFar; }
    void setPerspectiveFov(float fov) { m_perspectiveFov = fov; calculateProjection(); }
    void setPerspectiveNear(float near) { m_perspectiveNear = near; calculateProjection(); }
    void setPerspectiveFar(float far) { m_perspectiveFar = far; calculateProjection(); }

    float getAspect() const { return m_aspect; }

    bool primary = false;

private:
    Camera::ProjectionType m_projectionType = Camera::ProjectionType::Orthographic;

    float m_orthoSize = 10.f;
    float m_orthoNear = -1.f;
    float m_orthoFar = 1.f;

    float m_perspectiveFov = math::radians(45.f);
    float m_perspectiveNear = 0.01f;
    float m_perspectiveFar = 100.f;

    float m_aspect = 0.f;
};

}
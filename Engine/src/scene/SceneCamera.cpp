#include <scene/SceneCamera.h>
#include <maths/matrix/matrix_transform.h>
#include <scene/Scene.h>

namespace Engine
{

SceneCamera::SceneCamera()
{
    calculateProjection();
}

SceneCamera::~SceneCamera()
{
    
}

void SceneCamera::onSceneSet(const Reference<Scene>& scene)
{
    setViewportSize(scene->getViewportSize().x, scene->getViewportSize().y);
}

void SceneCamera::setOrthographic(float size, float near, float far)
{
    m_projectionType = Camera::ProjectionType::Orthographic;

    m_orthoSize = size;
    m_orthoNear = near;
    m_orthoFar = far;
    
    calculateProjection();
}

void SceneCamera::setPerspective(float verticalFov, float near, float far)
{
    m_projectionType = Camera::ProjectionType::Perspective;

    m_perspectiveFov = verticalFov;
    m_perspectiveNear = near;
    m_perspectiveFar = far;

    calculateProjection();
}

void SceneCamera::calculateProjection()
{
    if (m_projectionType == Camera::ProjectionType::Perspective)
    {
        m_projectionMatrix = math::perspective(m_perspectiveFov, m_aspect, m_perspectiveNear, m_perspectiveFar);
    }
    else
    {
        float left = -m_orthoSize * m_aspect * 0.5f;
        float right = m_orthoSize * m_aspect * 0.5f;
        float bottom = -m_orthoSize * 0.5f;
        float top = m_orthoSize * 0.5f;

        m_projectionMatrix = math::ortho(left, right, bottom, top, m_orthoNear, m_orthoFar);
    }
}

void SceneCamera::setViewportSize(uint32_t width, uint32_t height)
{
    m_aspect = static_cast<float>(width) / static_cast<float>(height);
    calculateProjection();
}

}
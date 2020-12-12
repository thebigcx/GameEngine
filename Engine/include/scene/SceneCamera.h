#pragma once

enum class ProjectionType
{
    Perspective,
    Orthographic
};

class SceneCamera
{
public:
    SceneCamera() {};

    ProjectionType getProjectionType() const
    {
        return m_projectionType;
    }

private:
    ProjectionType m_projectionType = ProjectionType::Orthographic;
};
#pragma once

#include <maths/math.h>

#include <events/Event.h>
#include <events/EventDispatcher.h>

class OrthographicCamera
{
public:
    OrthographicCamera();

    void setPosition(const math::vec2& position);
    void translate(const math::vec2& vec);

    inline const math::vec2& getPosition() const { return m_position; }

    void onEvent(Event& event);
    void onWindowResize(WindowResizeEvent& event);

    math::mat4 getViewMatrix() const;
    inline const math::mat4& getProjectionMatrix() const
    {
        return m_projectionMatrix;
    }

protected:
    math::vec2 m_position;
    math::mat4 m_projectionMatrix;
};
#pragma once

#include <maths/math.h>

#include <events/Event.h>
#include <events/EventDispatcher.h>
#include <util/Camera.h>

namespace Engine
{

class OrthographicCamera : public Camera
{
public:
    OrthographicCamera();
    virtual ~OrthographicCamera() = default;

    virtual void setPosition(const math::vec2& position);
    virtual void translate(const math::vec2& vec);
    virtual void translate(float x, float y);

    inline const math::vec2& getPosition() const { return m_position; }

    virtual void onEvent(Event& event);
    virtual void onWindowResize(WindowResizeEvent& event);

    math::mat4 getViewMatrix() const;

protected:
    math::vec2 m_position;
};

}
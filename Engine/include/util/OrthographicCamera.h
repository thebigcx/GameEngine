#pragma once

#include <maths/math.h>

#include <util/Camera.h>

namespace Engine
{

class OrthographicCamera : public Camera
{
public:
    OrthographicCamera();
    virtual ~OrthographicCamera() = default;

    virtual void setPosition(const math::vec3& position);
    virtual void setPosition(float x, float y, float z);

    virtual void translate(const math::vec3& vec);
    virtual void translate(float x, float y, float z);
    
    virtual void setRotation(float rotation);
    virtual void rotate(float rotation);

    inline const math::vec3& getPosition() const { return m_position; }
    inline float getRotation() const { return m_rotation; }

    void setProjection(uint32_t left, uint32_t right, uint32_t bottom, uint32_t top);

protected:
    math::vec3 m_position;
    float m_rotation = 0;

    void recalculateView();
};

}